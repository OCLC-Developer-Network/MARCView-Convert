# MARCView-Convert

Source code for the MARCView-Convert

## Installation MARCView

Executable for MARCView is attached to the [most recent release] (https://github.com/OCLC-Developer-Network/MARCView-Convert/releases/download/3.12.2/MARCView.exe)

## Installation MARC-Convert
This must be compiled from source code as every installation is unique

## Application Summary

MARCView and MARConvert are two products generated from the same C++ codebase.
We offer this code and documentation to the community free and without obligation,
under the Apache 2.0 license.

### MARCView

MARCView is an easy-to-use Windows program to view, search, and print any file of
MARC21, UNIMARC, or MARCXML bibliographic or authority records. Records are
formatted for easy viewing. Although MARCView handles files of any number of
records, searching is linear, so becomes slower for large files; about a million records is
the practical limit for searching. The size of the file must be less than 2 GB.

MARCView a simple file viewer. As such, it tries to read anything readable and does not
ask the user much. For example, it ignores characters between records if it can. It does
not report character conversion problems since the user can't do anything about them and
they won't interfere with viewing. It does not report minor validation errors.
This simplicity means that certain limitations are hard to fix.

### MARConvert

MARConvert is a Windows application used to make custom conversions, especially
when other programs were not customizable enough. Typically Systems Planning would
modify the code for each client. Since we could modify the source code, virtually any
kind of conversion was possible.
