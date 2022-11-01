Development process of the Accellera SystemC CCI reference implementation
=========================================================================

This document focuses on the technical aspects related to the development
of the SystemC Configuration Control and Inspection (CCI) reference 
implementation. Legal and formal procedures are documented at 

http://accellera.org/about/policies-and-procedures

---------------------------------------------------------------------
Repository setup
---------------------------------------------------------------------

The central source code repository of the Accellera SystemC CCI reference 
implementation is hosted at [GitHub](https://github.com/accellera-official/).
The read-only repository can be found at

https://github.com/accellera-official/cci

### Creating a personal fork

In order to contribute changes to the repository, it is recommended to create
personal (or company-based) [forks][2] of the repository on GitHub and push 
the proposed changes (bugfixes, features, ...) there.  Details of the intended
work-flow are described in the next [section](#basic-branch-setup).  It is 
convenient to add this GitHub fork as a remote to your local clone of the 
repository:

```
cd <repo>/
git remote add origin git@github.com:<your-account>/<repo>.git
git branch --set-upstream master origin/master
```

Contributions to the Accellera SystemC CCI repository should comply with the
[contributing guidelines][1].

Any changes can then be pushed to GitHub using:

```
git push [options] [<repository>] [<refspec>...]
```

 * If you omit the `<repository>`, the default destination is
   the remote of the current branch (or `origin`).
 * The `<refspec>` basically follows the format
    `<local-branch>:<remote-branch>`, or just `<branch>`, if
   both are the same.
 * Omitting the `<refspec>` pushes all branches with 'matching'
   remote branches to the repository.

A basic cheat sheet containing the an overview of the general
Git commands and workflow can be found [online][3].

[1]: ../CONTRIBUTING.md "Contriubting to SystemC CCI"
[2]: https://docs.github.com/en/get-started/quickstart/fork-a-repo
[3]: http://zrusin.blogspot.de/2007/09/git-cheat-sheet.html "Git Cheat Sheet"


---------------------------------------------------------------------
Development flow
---------------------------------------------------------------------

### Basic branch setup

The main idea is to have two main branches, one for the development,
one for the releases.

* **master**

  The latest and greatest `HEAD` of the LWG development.
  This is where all the new features and fixes go.

* **release**

  This branch is used to create the release tarballs, both internal
  and public snapshots, typically based on the `public` branch.

For the **regressions**, the `release` branch is more or less just a pointer
to the latest revision of a snapshot (or release).  It is still useful
to keep a named branching point, in case of required hotfixes.

For the **cci library**, the `release` branch is to be different from the
`master` branch.  The idea is to fully track the contents of the released
tarball.  This requires the following changes compared to the `master`
branch:

  - The Automake generated files are added to this `release` branch.
  - Accellera internal files are stripped
    (`.gitignore`, internal documentation, ...).

To prepare a release, the `master` branch would then be merged into the
`release` branch, the automake files would be updated (if necessary) and
the clean working tree could be used as baseline for the tarball (e.g.,
via `git-archive(1)`).  Details are described in the next section
[release management](#release-management).  

If more sophisticated version branches are needed, a development
model similar to the well-known ["successful branching model"][4]
can be deployed.  Not all aspects of this model are expected to
be needed for the SystemC CCI reference implementation, as we usually
maintain only a single (i.e., the latest) public release of the
kernel.

[4]: http://nvie.com/posts/a-successful-git-branching-model/ "'A successful Git branching model' by Vincent Driessen"


### Adding a feature (set) or bug fix

The development of a new contribution in form of a feature or a
complex bug fix is best done in a new feature branch, which is
forked and checked out from the `master` branch:

```
git checkout -b <company>-<feature-xyz> master
```

Then code up the new contribution.  Please try to facilitate code
review by other Accellera members by logically grouping your changes into
one commit per addressed issue. For the commit messages, please
consider to follow these suggestions: 

>  *Note:* **Commit messages**
>
>  Though not required, it's a good idea to begin the commit message with
>  a single short (less than 50 character) line summarizing the change,
>  followed by a blank line and then a more thorough description. Tools
>  that turn commits into email, for example, use the first line on the
>  `Subject:` line and the rest of the commit in the body.

> *Note:* **Sign-off procedure for commits**
>
> In order to document that contributions are submitted under the
> Apache-2.0 license (see `LICENSE`), a sign-off procedure is
> defined in the [contributing guidelines][9].

During the development of the contribution, the `master` branch may
receive other commits. In that case, consider rebasing the commits in
your feature branch onto the `HEAD` of the `master` branch to keep the
history clean. Once the contribution is ready for review by the
working group, push the feature branch in your fork of the respective
repository on GitHub:

```
git push <your-github-fork-remote-name> <company>-<feature-xyz>
```

Then, send a [pull request][5] either manually or via [GitHub][5] to
initiate the code review by the Accellera working group members.  
The summary can be manually generated by

```
git request-pull master git@github.com/<account>/<repo>.git \
   <company-feature-xyz>
```

to be sent to the Accellera Working Group email reflector.

To review the proposed contributions, one can either browse the
repository at GitHub, or add the remote location to a local
clone of the repository

```
# add the fork to your set of "remotes"
git remote add <remote-name> git@github.com/<account>/<repo>.git
git fetch  <remote-name>

# examine differences
git diff master..<remote-name>/<company-feature-xyz>
git log <remote-name>/<company-feature-xyz>
```

After the contribution is accepted, it will be merged into the working group's
`master` branch by the responsible source code maintainer.  This should
be done with an explicit *merge commit*, to keep the individual 
contributions separated:

```
git merge --no-ff --log \
   <remote-name>/<company-feature-xyz>
```

Instead of fully merging the contribution, the maintainer may choose
to cherry-pick individual commits or to rebase the feature branch on
an intermittently updated `master`. He may also request additional
changes to be done by the submitter. In that case, the submitter may
need to merge recent changes to the `master` branch into his feature
branch before carrying out the requested changes.

After the contribution has been fully merged into `master`, the
feature branch in the local and Github fork may be deleted.

```
git branch -d <company-feature-xyz>      # delete local branch
git push  origin :<company-feature-xyz>  # delete remote branch
```

[5]: https://help.github.com/articles/using-pull-requests "Using Pull Requests - github:help"


### Maintaining a private (set of) branches

Vendors may be interested in maintaining their own, in-house flow
to align the internal development of a derived SystemC CCI reference 
implementation, while being able to pick fixes from the Accellera Working
Group's tree (and hopefully) contributing fixes and features back to the 
reference implementation.

For this purpose members may employ the already mentioned ["successful
branching model"][4] by Vincent Driessen. The vendor can branch its
own development branch, e.g., `develop-<vendor>` from the already
tracked working group development branch `master` in his clone of the WG
repository. The vendor is then able to integrate commits on the WG
development branch by merging it into his his vendor development
branch.

Bug fixes to be contributed back to the WG consist usually of one or
several isolated commits. They need to be cherry-picked from the
vendor's development branch into a new branch created from the WG
development branch:

```
git checkout -b <vendor>-fix-<bug> origin/master
git cherry-pick <commit>...
```

Once, the bug fix branch is ready, it should be pushed into the
vendor's github account and a pull request created, as described in
the [feature branch section](#adding-a-feature-set).

A new feature consists usually of a series of commits developed in a
dedicated feature branched of the vendor's or WG's development
branch. Only in the first case, a rebase on the top of the WG's
development branch is necessary. To this end, branch first from the
feature branch:

```
git checkout -b <vendor>-<new-feature> <private-feature-branch>
git rebase [-i|--interactive] --onto origin/master develop-<vendor>
```

Once, the bug fix branch is ready, it should be pushed into the
vendor's github account and a pull request created, as described in
the [feature branch section](#adding-a-feature-set).


---------------------------------------------------------------------
Versioning scheme
---------------------------------------------------------------------

In general, the versioning pattern for the SystemC CCI reference implementation
follows the scheme defined by the SystemC/TLM standard defined in 
IEEE Std. 1666-2011 (8.6.5), based on `Major.Minor.Patch` numbers and a date
(for prereleases).

*Note:* In general, no compatibility guarantees are attached to these version
        numbers, not even for reference implementation itself, to avoid burdens
        across different SystemC CCI reference implementations.

The individual parts of the SystemC CCI versions reported via the corresponding
header files should be handled according to the following criteria:

 * Update **major version** only together with
   * IEEE 1666 update
   * and/or major new language features (e.g. in pre-releases for IEEE update)
 * Update **minor version** for new Accellera standard release
   * Significant language extension proposals to be added to next IEEE release
 * Update **patch version** for minor API changes
 * Update `CCI_VERSION` date macro after every pull-request merge
   * ISO8601 date format (YYYYMMDD)
   * Done by the maintainers of the reference implementation 

*Note:* Changes only affecting documentation, examples, or the build system
        should not lead to a change of the version
        (not even changing `CCI_VERSION` macro).

The overall release date (see [next section](#release-management)) of the
release tarball of the reference implementation should reflect the date of the
archive creation.

---------------------------------------------------------------------
Release management
---------------------------------------------------------------------

To prepare a new release tarball, the following set steps are to be
performed by the maintainer

1. **Prepare the release in the `master` branch**

   Before creating a release snapshot, the documentation and version
   information in the package should be updated within the `master`
   branch.  This includes files like
   - `ChangeLog`, `RELEASENOTES`, `README`, `INSTALL`
   - [Version headers](#versioning-scheme)

   During the release preparation phase, other functional changes
   should not be added/merged to the `master` branch.

2. **Update the `release` branch**

   ```
   # switch to release branch
   git checkout release

   # merge master branch
   git merge --no-commit master
   git rm <new-internal-file...> # drop new or changed "private" files
   git commit -m "merge master branch for x.x.x release"
   ```

3. **Update the Autoconf (and other auto-generated) files**

   ```
   autoreconf -if # or config/bootstrap
   git add -u     # add changed files
   git status     # check for untracked files
   git add <new files to distribute>
   git commit -m "update autogenerated files for x.x.x release"
   ```

4. **Tag the release revision**

   In order to keep track of the release snapshots, the revisions used
   for creating the release tarballs should be marked with an *annotated*
   and optionally signed Git tag.

   ```
   # git tag -a -m "<package> <version>" <version> <refspec>
   git tag -a -m "CCI 1.0.1" 1.0.1 release
   ```

   The tagname should contain the `<version>` in `Major.Minor.Patch` format
   (see [section Versioning scheme](#versioning-scheme)). Optionally, the 
   following extensions could be added to the release name:
   * `x.x.x_rc_<isodate>` for release candidate (version for Accellera WG internal review)
   * `x.x.x_beta_<isodate>` for beta/internal versions
   * `x.x.x_pub_rev_<isodate>` for public review versions, and
   * `x.x.x` for public release versions.

   > *NOTE:* The tag should be on the `release` branch, to enable the
   > automated tarball creation in the next step.

5. **Create the release tarball**

   `git archive` can then be used to create the release tarball.
   `git describe` can be used to obtain the correct tarball name
   based on the current tag.

   ```
   PACKAGE="`basename $(git rev-parse --show-toplevel)`" # or directly 'cci'
   VERSION="`git describe release`"
   git archive -o ${PACKAGE}-${VERSION}.tgz \
      --prefix=${PACKAGE}-${VERSION}/ release
   ```

   > *NOTE:* Even without a tag, a quick-shot release of the
   >         release branch can be generated this way.

6. **Publish the release**

   Upload the archive to the Accellera Working Group workspace for internal
   review and push the changes to GitHub.

   ```
   git push accellera-official master release <version>
   ```

   > *NOTE:* The tag needs to be pushed explicitly.

   > *NOTE:* For each tag pushed to the GitHub repository, the corresponding
             release archive can be downloaded from GitHub directly via
             `https://github.com/accellera-official/cci/releases/tag/<version>`.


---------------------------------------------------------------------
Issue tracking
---------------------------------------------------------------------

Open issues (bugs, cleanups, features) related to the reference
implementation of SystemC CCI library are tracked via GitHub:

 * <https://github.com/accellera-official/cci/issues>

Labels are used to classify issues according to their
severity (10 highest), according to the following guidelines:

 * `10-critical`  
   Show-stoppers that must be fixed, affects all (or at least most)
   platforms and violates fundamental specifications for most applications.

 * `09-serious`  
   At least one of the explicitly supported platforms is affected and
   causes significant problems for many applications.
 
 * `06-medium`  
   Covers an area, where the standard may not be clearly specified.
   May require changes to external/standard API.
 
 * `05-feature`  
   New feature proposal, beyond the current standard. Includes internal
   (and external, providing adoption by IEEE P1666 WG) API changes.
 
 * `04-errata`  
   Inconvenience (errata) for users of many platforms, workaround available.
   Solution may require internal API changes.

 * `02-documentation`  
   Documentation inconsistency or insufficiency (e.g. whitepaper unclear
   or misleading), no code changes.

 * `01-inconvenience`  
   Inconvenience (workaround available), for some platforms
   (e.g. users of Visual Studio 2003)
 
 * `00-cosmetic`  
   Changes addressing performance or clarity of implementation,
   no API changes.

The discussion on issues usually starts on the Accellera Working Group 
email reflector or during the working group meetings.  After an initial 
consensus on the "validity" of the issue, the issue is added to the 
issue tracking system, a classification is done (including a target 
milestone), and preferably a responsible person is assigned.
