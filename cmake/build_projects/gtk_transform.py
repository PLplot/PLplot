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

def parse_jhbuild(root, id, if_dependencies):
    # find all necessary data to build.  If xml is in slightly non-standard
    # form or if there is any other trouble, then immediately return
    # with no output to stdout.
    print("id = %s" % id)
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
        config_arguments =  package.get("autogenargs")
    elif config_type == "cmake":
        config_arguments =  package.get("cmakeargs")
    else:
        return None
    
    # Parse branch element of package
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
    index = download_hash.index(":")
    download_hash_type = download_hash[0:index].upper()
    download_hash = download_hash[index+1:]
    
    download_repo = branch.get("repo")
    if len(download_repo) == 0:
        return None
    
    download_module = branch.get("module")
    if len(download_module) == 0:
        return None
    
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

    # Create overall_dependencies dictionary and populate it as needed.
    overall_dependencies={}
    # Create dependencies dictionary and populate it as needed.
    dependencies={}
    # Add a dependency for pkg-config if there is a subelement named that.
    if package.find("pkg-config") != None:
        dependencies["pkg-config"] = None
        overall_dependencies["pkg-config"] = None
    for dep_element in package.findall("dependencies/dep"):
        dependencies[dep_element.get("package")] = None
        overall_dependencies[dep_element.get("package")] = None

    # Create suggests dictionary and populate it as needed.
    suggests={}
    for dep_element in package.findall("suggests/dep"):
        suggests[dep_element.get("package")] = None
        overall_dependencies[dep_element.get("package")] = None
    # Repeat for suggest target name (a mistake for one package
    # with id=gdm).
    for dep_element in package.findall("suggest/dep"):
        suggests[dep_element.get("package")] = None
        overall_dependencies[dep_element.get("package")] = None
        
    # Create after dictionary and populate it as needed.
    after={}
    for dep_element in package.findall("after/dep"):
        after[dep_element.get("package")] = None
        overall_dependencies[dep_element.get("package")] = None
    
    if not if_dependencies:
        pass
    else:
        dependencies_list = overall_dependencies.keys()
        bad_dependencies = {}
        for dep in dependencies_list:
            extra = parse_jhbuild(root, dep, True)
            if extra == None:
                bad_dependencies[dep] = None
                del overall_dependencies[dep]
            else:
                overall_dependencies.update(extra[0]) 
                bad_dependencies.update(extra[1]) 

        return (overall_dependencies, bad_dependencies)

tree = ET.parse(sys.stdin)
root = tree.getroot()

# Create repository dictionary.  Note, there is sometimes more than
# one definition.  Eyeballing the file, it appears those definitions
# are completely redundant so it is fine to take the last definition
# (which we do here).
repository_dict={}
for repository in root.findall("repository"):
    repository_dict[repository.get("name")] = repository.get("href")

dependency_dictionary = parse_jhbuild(root, "pango", True)
if not dependency_dictionary == None:
    print("good dependencies =")
    print(dependency_dictionary[0].keys())
    print("bad dependencies =")
    print(dependency_dictionary[1].keys())
