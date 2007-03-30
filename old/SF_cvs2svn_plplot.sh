#!/bin/sh

# SF_cvs2svn.sh -- shell script to automate most of the process of
# converting SourceForge CVS repositories to SVN repositories and doing some
# basic sanity checks on the results.

### Edit the next 5 export lines as appropriate.
# All file changes will occur in ${PROJECT} subdirectories of this prefix
# directory
export PROJECT_PREFIX=/home/irwin/subversion_conversion

# Change PROJECT for each different SourceForge project you need
# to convert to subversion.
export PROJECT=plplot

# TAGS is a list of the tags where you want to check file contents
# between the CVS and SVN versions.  I used all tags that could be obtained
# with svn list here with an old 'throwaway' cvs* tag that somehow has
# survived the perl script that was supposed to delete the 
# throwaway tags.  Also, all obvious release candidate tags are dropped.

export TAGS=\
'AT_mc1 '\
'CFDIR_HEAD_bp '\
'before_ATmerge '\
'bp_AM-LT '\
'bp_AT '\
'bp_DEBIAN '\
'bp_GNOME '\
'bp_SHADE-EXCLUSION '\
'bp_dyndrv_1 '\
'bp_lss_1 '\
'bp_mjl_fork_1 '\
'bp_tea '\
'bp_v5_0 '\
'debian-woody-bp '\
'debian_4-99j_11 '\
'debian_5-0-1_0-potato-1 '\
'debian_5-0-1_0-potato-2 '\
'debian_5-0-1_0-potato-3 '\
'debian_5-0-1_0-potato-4 '\
'debian_5-0-2_0-potato-1 '\
'debian_5-0-2_0-potato-3 '\
'merge_debian_v5-0-1 '\
'mp_MAIN_AM-LT_1 '\
'mp_MAIN_AM-LT_2 '\
'v4p99d '\
'v4p99e '\
'v5_0_0 '\
'v5_0_1 '\
'v5_0_2 '\
'v5_0_3 '\
'v5_0_4 '\
'v5_1_0_final '\
'v5_2_0_final '\
'v5_2_1_final '\
'v5_3_0_final '\
'v5_3_1 '\
'v5_5_0 '\
'v5_5_1 '\
'v5_5_2 '\
'v5_5_3 '\
'v5_6_0 '\
'v5_6_1 '\
'v5_7_0 '\
'v5_7_1 '\
'v5_7_2 '\
'v5_7_3 '\
''

# Branches is a list of branches that you want to
# (a) check head of branch file contents between CVS and SVN version.
# (b) check ChangeLog for entire branch between CVS and SVN versions.
# This list was created with the svn list command.  I have investigated
# all the unlabeled branches further, and they were all created (for
# unknown reasons) by Rafael Laboissiere for a few files he was editing in 
# 2004 and at least the first one cannot be extracted from CVS by the -r
# option even though cvs2svn was able to see it.  Therefore, I feel these
# are a result of some CVS inconsistency so I will ignore them.
#'unlabeled-1.1.16 '\
#'unlabeled-1.10.2 '\
#'unlabeled-1.11.2 '\
#'unlabeled-1.12.2 '\
#'unlabeled-1.13.2 '\
#'unlabeled-1.150.2 '\
#'unlabeled-1.2.12 '\
#'unlabeled-1.2.2 '\
#'unlabeled-1.2.4 '\
#'unlabeled-1.32.2 '\
#'unlabeled-1.36.2 '\
#'unlabeled-1.4.2 '\
#'unlabeled-1.43.2 '\
#'unlabeled-1.5.2 '\
#'unlabeled-1.5.4 '\
#'unlabeled-1.6.2 '\
#'unlabeled-1.69.2 '\
#'unlabeled-1.7.2 '\
#'unlabeled-1.74.2 '\

# Also, I did a preliminary run to obtain the starting revision number of
# each branch (after the colon) to cut down on the duplicated diffs to check.

export BRANCHES_REVISIONS=\
'AM-LT:2998 '\
'AT:4054 '\
'CFDIR:5359 '\
'DEBIAN:2474 '\
'GNOME:2665 '\
'SHADE-EXCLUSION:2731 '\
'airwin:2147 '\
'bp_pstex_driver:3017 '\
'debian-woody:5649 '\
'dyndrv_1:2873 '\
'enhance_3dplots:3982 '\
'lss_1:3547 '\
'mjl_fork_1:6216 '\
'plplot4p99f:608 '\
'rlaboiss:2228 '\
'sgml:2222 '\
'tea:2124 '\
'v5_0:2105 '\
'v5_3_0:5245 '\
''

# Change MODULE to the name of the module you are converting for your
# particular SF project.  It is often the case that MODULE=${PROJECT}
# but in the case of PROJECT=plplot there is a module called plplot
# and a module called www, in which case this script must be called twice
# with MODULE=plplot and MODULE=www.
export MODULE=${PROJECT}
export MODULE1=www

# tree diff options so that windows line endings won't mess up the comparison
# (--strip-trailing-cr) and file lines with $Id, $Revision (and Revision which
# $Revision spits out on a separate line), $Log, $Date, and
# $Name will be skipped since those automatic file modifiers give different
# results (Revision numbers and date format) for subversion.
export TREE_DIFF_OPTIONS=\
'-Naur -I \$Id -I \$Revision -I Revision  -I \$Log -I \$Date '\
'-I \$Name --strip-trailing-cr'

# N.B. this is the assumed encoding for the CVS commit messages.  Change to
# the encoding you used for your CVS commits.  It is often the case that
# CODING='iso-8859-1'.  This covers the case where commits were by those 
# who use western European languages.
export CODING='iso-8859-1'

### Everything below this line should not need editing.

# Create module directory (if needed) and cd to it.
mkdir -p ${PROJECT_PREFIX}/${MODULE}
cd ${PROJECT_PREFIX}/${MODULE}

# rsync the SF CVS repository of ${PROJECT}.
rsync -av --delete \
rsync://${PROJECT}.cvs.sourceforge.net/cvsroot/${PROJECT}/* \
${PROJECT}-cvsbackup

tar zcf ${PROJECT}-cvsbackup.tar.gz ${PROJECT}-cvsbackup

# Create svn dumpfile from local CVS repo.  Note this ignores CVSROOT module
# which is fine.  May have to get creative later with multiple module results
# such as plplot and www.
# N.B. Could specify 
#  --trunk=${MODULE}/trunk \
#  --tags=${MODULE}/tags \
#  --branches=${MODULE}/branches \
# options to put ${MODULE} at top of dump file hierarchy above TTB, but
# it appears from "svn list" experiments with CVS lasi SVN repository that
# the TTB directories should be at top of dump file hierarchy.
rm -f svndump cvs2svn.out
cvs2svn --encoding=${CODING} --dumpfile=svndump \
${PROJECT}-cvsbackup/${MODULE}  >& cvs2svn.out

# Create an empty local subversion repository for ${MODULE}.
rm -rf ${PROJECT_PREFIX}/${MODULE}/subversion_repository
svnadmin create ${PROJECT_PREFIX}/${MODULE}/subversion_repository

# Populate that repository.

rm -f ${MODULE}_load.out
svnadmin load \
${PROJECT_PREFIX}/${MODULE}/subversion_repository \
< svndump >& ${MODULE}_load.out

# checkout both kinds of repo from local repositories

rm -rf ${MODULE}_cvs ${MODULE}_cvs_checkout.out
cvs -d \
${PROJECT_PREFIX}/${MODULE}/${PROJECT}-cvsbackup/ \
checkout -d ${MODULE}_cvs ${MODULE} >& ${MODULE}_cvs_checkout.out

rm -rf ${MODULE}_svn ${MODULE}_svn_checkout.out
svn checkout \
file://${PROJECT_PREFIX}/${MODULE}/subversion_repository/trunk \
${MODULE}_svn >& ${MODULE}_svn_checkout.out

# Check 0: Confirm top-level svn repository directories are TTB
svn list file://${PROJECT_PREFIX}/${MODULE}/subversion_repository

# Check 1: Does fresh checkout of both kinds of repo give the same results?
diff ${TREE_DIFF_OPTIONS} --exclude=CVS --exclude=.svn \
${MODULE}_cvs ${MODULE}_svn

# Check 2: Are the commit messages the same?
# Use uniconv to convert from ${CODING} to utf-8,
# remove xmlns from result using sed transform, and
# use xslt to transform resulting xml to formatted text output that
# is in the same format as the svn2cl results (which are also done
# with xslt).
cd ${MODULE}_cvs
cvs2cl -l "-d<=now" --stdout --gmt --xml --show-dead  --follow trunk| \
uniconv -decode ${CODING} -encode 'utf-8' | \
sed 's? xmlns=".*"??' | \
xsltproc --stringparam reparagraph yes ${PROJECT_PREFIX}/cvs2cl.xsl - >| \
../ChangeLog_cvs
cd ..

cd ${MODULE}_svn
svn2cl --reparagraph --stdout >| ../ChangeLog_svn
cd ..

diff -au ChangeLog_cvs ChangeLog_svn

# Check 3: Are the tagged versions identical between CVS and SVN?
for tag in $TAGS ; do 
  echo "taking CVS-SVN diff of tag = ${tag}" 
  rm -rf ${MODULE}_cvs_export ${MODULE}_cvs_export.out
  cvs -d \
    ${PROJECT_PREFIX}/${MODULE}/${PROJECT}-cvsbackup/ \
    export -r ${tag} -d ${MODULE}_cvs_export ${MODULE} \
    >& ${MODULE}_cvs_export.out

  rm -rf ${MODULE}_svn_export ${MODULE}_svn_export.out
  svn export \
    file://${PROJECT_PREFIX}/${MODULE}/subversion_repository/tags/${tag} \
    ${MODULE}_svn_export >& ${MODULE}_svn_export.out

  diff ${TREE_DIFF_OPTIONS} ${MODULE}_cvs_export ${MODULE}_svn_export
done

# Check 4: Are the Head of branches identical between CVS and SVN?
for branch_revision in $BRANCHES_REVISIONS ; do
  branch=${branch_revision%:*}
  echo "taking CVS-SVN diff of branch = ${branch}"
  rm -rf ${MODULE}_cvs_export ${MODULE}_cvs_export.out
  cvs -d \
    ${PROJECT_PREFIX}/${MODULE}/${PROJECT}-cvsbackup/ \
    export -r ${branch} -d ${MODULE}_cvs_export ${MODULE} \
    >& ${MODULE}_cvs_export.out

  rm -rf ${MODULE}_svn_export ${MODULE}_svn_export.out
  svn export \
    file://${PROJECT_PREFIX}/${MODULE}/subversion_repository/branches/${branch} \
    ${MODULE}_svn_export >& ${MODULE}_svn_export.out

  diff ${TREE_DIFF_OPTIONS} ${MODULE}_cvs_export ${MODULE}_svn_export
done

# Check 5: Are the ChangeLogs for branches identical between CVS and SVN?
for branch_revision in $BRANCHES_REVISIONS ; do
  branch=${branch_revision%:*}
  revision=${branch_revision#*:}
  echo "taking CVS-SVN ChangeLog diff for branch = ${branch}"
  echo "from revision ${revision} onward."

  rm -rf ${MODULE}_svn_branch ${MODULE}_svn_branch.out
  svn checkout \
    file://${PROJECT_PREFIX}/${MODULE}/subversion_repository/branches/${branch} \
    ${MODULE}_svn_branch >& ${MODULE}_svn_branch.out

  cd ${MODULE}_cvs
  cvs2cl -l "-d<=now" --stdout --gmt --xml --show-dead  \
  --follow-only ${branch} --no-ancestors | \
  uniconv -decode ${CODING} -encode 'utf-8' | \
  sed 's? xmlns=".*"??' | \
  xsltproc --stringparam reparagraph yes ${PROJECT_PREFIX}/cvs2cl.xsl - >| \
  ../ChangeLog_cvs_branch
  cd ..

  cd ${MODULE}_svn_branch
  svn2cl --revision HEAD:${revision} --reparagraph --stdout \
  >| ../ChangeLog_svn_branch
#  Find revision number corresponding to start of branch
#  svn log |grep -B 2 'This commit was manufactured by cvs2svn'
  cd ..

  diff -au ChangeLog_cvs_branch ChangeLog_svn_branch

done

# Checks for ${MODULE1}

rm -f svndump_${MODULE1} cvs2svn_${MODULE1}.out
cvs2svn --encoding=${CODING} --dumpfile=svndump_${MODULE1} \
--trunk=${MODULE1}/trunk \
--tags=${MODULE1}/tags \
--branches=${MODULE1}/branches \
${PROJECT}-cvsbackup/${MODULE1}  >& cvs2svn_${MODULE1}.out

# Populate the existing repository.

rm -f ${MODULE1}_load.out
svnadmin load \
${PROJECT_PREFIX}/${MODULE}/subversion_repository \
< svndump_${MODULE1} >& ${MODULE1}_load.out

# checkout both kinds of repo from local repositories

rm -rf ${MODULE1}_cvs ${MODULE1}_cvs_checkout.out
cvs -d \
${PROJECT_PREFIX}/${MODULE}/${PROJECT}-cvsbackup/ \
checkout -d ${MODULE1}_cvs ${MODULE1} >& ${MODULE1}_cvs_checkout.out

rm -rf ${MODULE1}_svn ${MODULE1}_svn_checkout.out
svn checkout \
file://${PROJECT_PREFIX}/${MODULE}/subversion_repository/${MODULE1}/trunk \
${MODULE1}_svn >& ${MODULE1}_svn_checkout.out

# Check 0: Confirm top-level svn repository directories are TTB
svn list file://${PROJECT_PREFIX}/${MODULE}/subversion_repository/${MODULE1}

# Check 1: Does fresh checkout of both kinds of repo give the same results?
diff ${TREE_DIFF_OPTIONS} --exclude=CVS --exclude=.svn \
${MODULE1}_cvs ${MODULE1}_svn

# Check 2: Are the commit messages the same?
# Use uniconv to convert from ${CODING} to utf-8,
# remove xmlns from result using sed transform, and
# use xslt to transform resulting xml to formatted text output that
# is in the same format as the svn2cl results (which are also done
# with xslt).
cd ${MODULE1}_cvs
cvs2cl -l "-d<=now" --stdout --gmt --xml --show-dead  --follow trunk| \
uniconv -decode ${CODING} -encode 'utf-8' | \
sed 's? xmlns=".*"??' | \
xsltproc --stringparam reparagraph yes ${PROJECT_PREFIX}/cvs2cl.xsl - >| \
../ChangeLog_${MODULE1}_cvs
cd ..

cd ${MODULE1}_svn
svn2cl --reparagraph --stdout >| ../ChangeLog_${MODULE1}_svn
cd ..

diff -au ChangeLog_${MODULE1}_cvs ChangeLog_${MODULE1}_svn
