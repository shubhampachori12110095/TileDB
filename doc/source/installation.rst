Installation
============

.. note::
    The easiest way to try out TileDB is to use a pre-built Docker image::

        docker pull tiledb/tiledb
        docker run -it tiledb/tiledb
    
    which uses the latest TileDB version, or:: 

        docker pull tiledb/tiledb:<version>
        docker run -it tiledb/tiledb:<version>

    which uses a specific TileDB version (ex. ``<version>`` could be ``1.2.0``). 

    More info at `TileDB Docker Hub repo <https://hub.docker.com/r/tiledb/tiledb/>`_ and the `TileDB-Docker repo <https://github.com/TileDB-Inc/TileDB-Docker>`_.

Pre-built Packages
------------------

Homebrew
~~~~~~~~

TileDB can be installed easily using the Homebrew package manager for macOS. Install instructions for Homebrew are provided on the `package manager's website <https://brew.sh/>`_.

To install the latest stable version of TileDB::

    brew update
    brew install tiledb-inc/stable/tiledb

HDFS and S3 backends are not enabled by default.
To build / install with one or more backends, use the ``--with-`` switch to enable them::

    brew install tiledb-inc/stable/tiledb --with-s3
    brew install tiledb-inc/stable/tiledb --with-hdfs

A full list of build options can be viewed with the ``info`` command::

    brew info tiledb-inc/stable/tiledb

To upgrade to the latest stable version of TileDB::

    brew upgrade tiledb-inc/stable/tiledb

To uninstall TileDB::

    brew uninstall tiledb-inc/stable/tiledb

Homebrew Tap is located at https://github.com/TileDB-Inc/homebrew.

Conda
~~~~~

A package for TileDB is available for the `Conda package manager <https://conda.io/docs/>`_. Conda makes it easy to install software into separate distinct environments on Windows, Linux, and macOS.

::

    conda install -c conda-forge tiledb

**Note**: Conda packages are not currently built with the HDFS and S3 storage backends enabled.

If you are compiling / linking against the TileDB conda package,
you need to explicity add the conda env path after activation:

::

    export CPATH=$CONDA_PREFIX/include
    export LIBRARY_PATH=$CONDA_PREFIX/lib
    export LD_LIBRARY_PATH=$CONDA_PREFIX/lib

Or as command line flags during compilation:

::

    gcc -I$CONDA_PREFIX/include -L$CONDA_PREFIX/lib -ltiledb tiledb_example.cc -o tiledb_example

Windows Binaries
~~~~~~~~~~~~~~~~

The easiest way to install TileDB on Windows is to download the .zip file containing pre-built Windows binaries from the `latest TileDB release <https://github.com/TileDB-Inc/TileDB/releases>`_. You can then simply configure your project (if using Visual Studio) according to the :ref:`Windows usage <windows-usage>` instructions.

Requirements
------------

Operating Systems
~~~~~~~~~~~~~~~~~

TileDB has been tested on **Ubuntu Linux** (v.14.04+), **CentOS Linux** (v.7+), **macOS Sierra** (v.10.12) and **Windows** (7+), but TileDB should work with any reasonably recent version of Ubuntu, CentOS, macOS or Windows with an installed compiler supporting C++11.

Required Dependencies
~~~~~~~~~~~~~~~~~~~~~

TileDB requires a recent version (3.3 or later) of the `CMake <https://cmake.org/>`_ build system.

TileDB has minimal required dependencies and currently relies on the following libraries for compression:

* `zlib <https://zlib.net/>`_
* `LZ4 <http://lz4.github.io/lz4/>`_
* `bzip2 <http://www.bzip.org/>`_
* `Zstandard <http://facebook.github.io/zstd/>`_
* `Blosc <http://blosc.org/pages/blosc-in-depth/>`_

When building from source, TileDB will locate these dependencies if already installed on your system, and locally install (not system-wide) any of them that are missing.

Optional Dependencies
~~~~~~~~~~~~~~~~~~~~~

S3
^^

Backend support for S3 stores requires the `AWS C++ SDK <https://github.com/aws/aws-sdk-cpp>`__. Similarly to the required dependencies, the TileDB build system will install the SDK locally if it is not already present on your system (when the S3 build option is enabled).

TileDB also integrates well with the S3-compliant `minio <https://minio.io>`__ object store.

HDFS
^^^^

Backend support for the Hadoop File System `HDFS <http://hadoop.apache.org/docs/current/hadoop-project-dist/hadoop-hdfs/HdfsDesign.html>`_ is optional. TileDB relies on the C interface to HDFS provided by `libhdfs <http://hadoop.apache.org/docs/current/hadoop-project-dist/hadoop-hdfs/LibHdfs.html>`_ to interact with the distributed filesystem.

During the build process the following environmental variables must be set:

* ``JAVA_HOME``: Path to the location of the Java installation.
* ``HADOOP_HOME``: Path to the location of the HDFS installation.
* ``CLASSPATH``: The Hadoop jars must be added to the CLASSPATH before running interacting with libhdfs.

Consult the `HDFS user guide <https://hadoop.apache.org/docs/current/hadoop-project-dist/hadoop-hdfs/HdfsUserGuide.html>`_ for installing, setting up, and using the distributed Hadoop file system.

Building from Source
--------------------

POSIX Systems
~~~~~~~~~~~~~

Begin by downloading a `release tarball <https://github.com/TileDB-Inc/TileDB/releases>`_ or by cloning the TileDB GitHub repo and checking out a release tag:

::

    git clone https://github.com/TileDB-Inc/TileDB
    git checkout <version>
    cd TileDB

where ``<version>`` is the version you wish to use (e.g., ``1.2.0``).

To configure TileDB, you can use the bootstrap script to run the CMake build generator::

    mkdir build
    cd build
    ../bootstap <flags>

You can also use the CMake command directly::

    mkdir build
    cd build
    cmake <flags> ..

The flags for the bootstrap script and the CMake equivalents are as follows:

=====================  ======================================================  ==============================
**Flag**               **Description**                                         **CMake Equivalent**
---------------------  ------------------------------------------------------  ------------------------------
``--help``             Prints command line flag options                        n/a
``--prefix=PREFIX``    Install files in tree rooted at ``PREFIX``              CMAKE_INSTALL_PREFIX=<PREFIX>
                       (defaults to ``TileDB/dist``)
``--dependency=DIRs``  Colon separated list to binary dependencies             CMAKE_PREFIX_PATH=<DIRs>
``--enable-debug``     Enable debug build                                      CMAKE_BUILD_TYPE=Debug
``--enable-coverage``  Enable build with code coverage support                 CMAKE_BUILD_TYPE=Coverage
``--enable-verbose``   Enable verbose status messages                          TILEDB_VERBOSE=ON
``--enable-hdfs``      Enables building with HDFS storage backend support      TILEDB_HDFS=ON
``--enable-s3``        Enables building with S3 storage backend support        TILEDB_S3=ON
``--disable-werror``   Disables building with the ``-Werror`` flag             TILEDB_WERROR=OFF
``--disable-cpp-api``  Disables building the TileDB C++ API                    TILEDB_CPP_API=OFF
=====================  ======================================================  ==============================

After configuring, run the generated make script::

    make -j <nprocs>

To build the examples run::

    make -C tiledb examples

To run the tests::

    make -C tiledb check

TileDB uses the `Catch <https://github.com/philsquared/Catch>`_ C++ unit test framework for testing.

Additional `command line flags <https://github.com/philsquared/Catch/blob/master/docs/command-line.md>`_ can be provided to the tiledb/test/tiledb_unit binary for controlling which tests are run and test output.

Windows
~~~~~~~

This section details how to build TileDB from source if you do not wish to use the precompiled DLLs from the .zip file attached to the TileDB releases.

Building TileDB on Windows has been tested to work with Microsoft Visual Studio 2017. You can install the free `Community Edition <https://www.visualstudio.com/vs/community/>`_ if you'd like the full IDE, or the `Build Tools <https://www.visualstudio.com/downloads/#Other%20Tools%20and%20Frameworks>`_ only if you don't need the IDE installed.

During the Visual Studio setup process, make sure the Git for Windows component is selected if you do not already have a working Git installation. Also be sure to select the CMake component if you do not have a working CMake installation.

In addition, you will need to install `PowerShell <https://docs.microsoft.com/en-us/powershell/>`_ (free).

To build and install TileDB, first open PowerShell and clone the TileDB repository::

    > git clone https://github.com/TileDB-Inc/TileDB
    > cd TileDB

Next, ensure the CMake binaries are in your path. If you installed Visual Studio, execute::

    > $env:Path += ";C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"

If you installed the build tools, instead execute::

    > $env:Path += ";C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"

Next create a build directory and configure TileDB::

    > mkdir build
    > cd build
    > ..\bootstrap.ps1 <flags>

You can also use the CMake command directly::

    > mkdir build
    > cd build
    > cmake <flags> ..

The flags for the bootstrap script and the CMake equivalents are as follows:

=====================  ================================================  ==============================
**Flag**               **Description**                                   **CMake Equivalent**
---------------------  ------------------------------------------------  ------------------------------
``-?``                 Display a usage message.                          n/a
``-Prefix``            Install files in tree rooted at ``PREFIX``        CMAKE_INSTALL_PREFIX=<PREFIX>
                       (defaults to ``TileDB\dist``)
``-Dependency``        Semicolon separated list to binary dependencies.  CMAKE_PREFIX_PATH=<DIRs>
``-CMakeGenerator``    Optionally specify the CMake generator string,    -G <generator>
                       e.g. "Visual Studio 15 2017". Check
                       'cmake --help' for a list of supported
                       generators.
``-EnableDebug``       Enable debug build                                CMAKE_BUILD_TYPE=Debug
``-EnableVerbose``     Enable verbose status messages.                   TILEDB_VERBOSE=ON
``-EnableS3``          Enables building with the S3 storage backend.     TILEDB_S3=ON
``-DisableWerror``     Disables building with the ``/WX`` flag           TILEDB_WERROR=OFF
``-DisableCppApi``     Disables building the TileDB C++ API              TILEDB_CPP_API=OFF
=====================  ================================================  ==============================

Note that the HDFS storage backend is not yet supported on Windows.

Finally, run the build::

    > cmake --build . --config Release

To run the tests::

    > cd tiledb
    > cmake --build . --target check --config Release

Or to build and install::

    > cd tiledb
    > cmake --build . --target install --config Release

Python Bindings
---------------

Build and install instructions for Python bindings can be found at the
`TileDB-Inc/TileDB-Py <https://github.com/TileDB-Inc/TileDB-Py>`_ repo.
