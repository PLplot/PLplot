#!/usr/bin/env python

# Read gtk jhbuild xml module that has been produced by
# gtk_xml_recursive_process.py from stdin and transform it into data
# required for build_projects using the ElementTree XML API.

# Copyright (C) 2013 Alan W. Irwin

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this file; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

import sys
import xml.etree.ElementTree as ET

def parse_jhbuild(root, id, depend_track, if_dependencies, called):
    # find all necessary data to build.  If xml is in slightly non-standard
    # form or if there is any other trouble, then immediately return
    # with no output to stdout.
    if called.has_key(id):
        raise RuntimeError, "parse_jhbuild recursively called with the same id"
    called[id] = None
    #sys.stderr.write("id = %s\n" % id)
    package = root.findall(".//*[@id='%s']" % id)
    if len(package) < 1:
        # Cannot find this package.  Do nothing.
        return None
    elif len(package) > 1:
        raise RuntimeError, "non-unique id found for %s" % id
    # From here on drop the reference to the first (and only) element
    # of package for convenience.
    package = package[0]

    # From now only handle two build types and only a subset of
    # attributes for both, and only look for
    # branch and dependencies elements (of various kinds) below
    # packages.
    config_type = package.tag
    if config_type == "autotools":
        config_arguments = package.get("autogenargs")
    elif config_type == "cmake":
        config_arguments = package.get("cmakeargs")
    elif config_type == "tarball":
        config_arguments = ""
    else:
        return None
    if config_arguments == None:
        config_arguments = ""
    # Parse branch or source element of package
    if config_type == "tarball":
        branch = package.findall("source[@hash]")
    else:
        branch = package.findall("branch[@hash]")
    if len(branch) < 1:
        # Cannot find branch so do nothing.
        return None
    elif len(branch) > 1:
        raise RuntimeError, "non-unique branch element found with hash attribute for %s" % id
    
    # From here on drop the reference to the first (and only) element
    # of branch for convenience.
    branch = branch[0]
    
    download_hash = branch.get("hash")
    if len(download_hash) == 0:
        return None
    index = download_hash.find(":")
    if index <= 0:
        download_hash_type = "UNKNOWN"
    else:
        download_hash_type = download_hash[0:index].upper()
    if index >= 0:
        download_hash = download_hash[index+1:]
    
    if config_type == "tarball":
        download_href = branch.get("href")
    else:
        download_repo = branch.get("repo")
        if len(download_repo) == 0:
            return None
    
        download_module = branch.get("module")
        if len(download_module) == 0:
            return None
        if repository_dictionary.has_key(download_repo):
            download_repo = repository_dictionary[download_repo]

        # Make sure there is a trailing "/" on the repo
        index = download_repo.rfind("/")
        if len(download_repo)-1 != index:
            download_repo = download_rep + "/"
        download_href = download_repo + download_module
    
    # Parse various kinds of jhbuild dependencies.
    # Note from 
    # http://stackoverflow.com/questions/9974957/what-is-the-after-element-used-for-in-jhbuild
    # "dependencies are hard dependencies. Packages that are
    # required to build a module.  suggests are soft
    # dependencies. Packages might use them if they are installed
    # (detected at build time) but if they are not present, they
    # do not present a problem. These dependencies will be built,
    # but they can be ignored without problems by using the
    # argument --ignore-suggests. For instance, evolution can be
    # built with or without nss support.  after are not strict
    # dependencies, but they are needed at runtime to get some
    # features. For instance, metacity is needed by mutter to have
    # key binding settings in mutter."

    # Create dependencies dictionary and populate it as needed.
    dependencies={}
    if depend_track&1:
        # Add a dependency for pkg-config if there is a subelement named that.
        if package.find("pkg-config") != None:
            dependencies["pkg-config"] = None
        for dep_element in package.findall("dependencies/dep"):
            dependencies[dep_element.get("package")] = None

    # Create suggests dictionary and populate it as needed.
    suggests={}
    if depend_track&2:
        for dep_element in package.findall("suggests/dep"):
            suggests[dep_element.get("package")] = None
        # Repeat for suggest target name (a mistake for one package
        # with id=gdm).
        for dep_element in package.findall("suggest/dep"):
            suggests[dep_element.get("package")] = None
        
    # Create after dictionary and populate it as needed.
    after={}
    if depend_track&4:
        for dep_element in package.findall("after/dep"):
            after[dep_element.get("package")] = None
    
    if if_dependencies:
        overall_dependencies = {}
        overall_dependencies.update(dependencies)
        overall_dependencies.update(suggests)
        overall_dependencies.update(after)

        good_packages = {}
        good_packages[id] = None
        bad_packages = {}
        dependencies_list = overall_dependencies.keys()
        for dep in dependencies_list:
            if called.has_key(dep):
                # ignore any package that has already been processed
                # by parse_jhbuild.  This avoids calling parse_jhbuild
                # twice for the case of two dependent packages
                # depending on a common third package and also ignores
                # circular dependencies.
                continue
            extra = parse_jhbuild(root, dep, depend_track, if_dependencies, called)
            if extra == None:
                bad_packages[dep] = None
            else:
                good_packages.update(extra[0]) 
                bad_packages.update(extra[1]) 

        return (good_packages, bad_packages)
    else:
        sys.stdout.write(id + "\n")
        sys.stdout.write(config_type + "\n")
        sys.stdout.write(config_arguments + "\n")
        sys.stdout.write(download_hash_type + "\n")
        sys.stdout.write(download_hash + "\n")
        sys.stdout.write(download_href + "\n")

        # Output dependency lists as sorted colon-separated strings
        dependencies_list = dependencies.keys()
        dependencies_list.sort()
        dependencies_string = ":".join(dependencies_list)
        sys.stdout.write(dependencies_string + "\n")
        suggests_list = suggests.keys()
        suggests_list.sort()
        suggests_string = ":".join(suggests_list)
        sys.stdout.write(suggests_string + "\n")
        after_list = after.keys()
        after_list.sort()
        after_string = ":".join(after_list)
        sys.stdout.write(after_string + "\n")

tree = ET.parse(sys.stdin)
root = tree.getroot()

# Create repository dictionary.  Note, there is sometimes more than
# one definition.  Eyeballing the file, it appears those definitions
# are completely redundant so it is fine to take the last definition
# (which we do here).
repository_dictionary={}
for repository in root.findall("repository"):
    repository_dictionary[repository.get("name")] = repository.get("href")

if len(sys.argv) < 3:
        raise RuntimeError, "must specify a starting package name as the first argument and dependency tracking flag as the second argument on the command line"
start_package = sys.argv[1]
# Important!
# The least significant bit of depend_track shows whether to pay
# attention to the dependencies attribute.
# The next least significant bit of depend_track shows whether to pay
# attention to the suggests attribute.
# The next least significant bit of depend_track shows whether to pay
# attention to the after attribute.
depend_track = int(sys.argv[2])
dependency_dictionary = parse_jhbuild(root, start_package, depend_track, True, {})
if dependency_dictionary == None:
    sys.stderr.write("some failure for start_package = %s or else no dependencies for that start_package\n" % start_package)
else:
    good_packages_list = dependency_dictionary[0].keys()
    bad_packages_list = dependency_dictionary[1].keys()
    good_packages_list.sort()
    bad_packages_list.sort()
    sys.stderr.write("number of good packages = %s\n" % len(good_packages_list))
    sys.stderr.write("good packages = " + ":".join(good_packages_list) + "\n")
    sys.stderr.write("number of bad packages = %s\n" % len(bad_packages_list))
    sys.stderr.write("bad packages = " + ":".join(bad_packages_list) + "\n")

    # Output on stdout good package results.
    for id in good_packages_list:
       parse_jhbuild(root, id, depend_track, False, {}) 
