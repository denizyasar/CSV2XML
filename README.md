# CSV2XML
Convert CSV to XML format

CSV stands for “comma separated value”, which is a data file format allowing data to
be saved in a tabular format where each column is separated with a comma. CSV
files generally have “csv” extension. CSV files can be used with most any
spreadsheet program, such as Microsoft Excel or Google Spreadsheets. In CSVs,
the separator may also be different characters, rather than comma. In this
assignment, we assume that the following characters are used as separator
character:

- comma (,)
- tab
- semicolon (;)

CSV2XML converter tool takes the name of the CSV input file assuming that is
encoded in UTF-8. You know that CSV flies are text file where different end of line
character may be used for different operating systems.
- In Windows system, CR+LF (carriage return and line feed) characters
- In Linux system, LF (line feed) character
- In MacOS classic system, CR (carriage return) character

## Usage
CSV2XML tool must take the following command line arguments:
- -separator
defines separator between each datum (1=comma, 2=tab, 3=semicolon)
- -opsys
defines the end of line character format (1=windows, 2=linux, 3=macos)
- -h
print information to screen about how other arguments can be used.

```cmd
CSV2XML <inputfile> <outputfile> [-separator <P1>][-opsys <P2>][-h]
```

The first argument, <input file> refers the source filename to be used for the
conversion and the second one, <outputfile>, refer the target XML filename. You
should take the input CSV filename as root node of the created XML.
P1 and P2 values can have the following values:
For P1 may be 1=comma, 2=tab or 3=semicolon, and P2 may be 1=windows,
2=linux or 3=macos

The sample command line usage converting from CSV to XML as follows:
```bash
CSV2XML Contacts.csv Contacts.xml -separator 1 -opsys 1
