# JSON_Lib
Parse JSON into an internal JSON Object (generating an exception if it is found not to be syntactically correct) so that it can be manipulated, interrogated and saved back to a text form (stringify). Parsing/stringification may occur to/from a byte buffer, file or a custom source/destination with the ISource and IDestination interfaces provided.

Stringification occurs with no whitespace being produced but if well formatted (indented) JSON is required then use the print and setIndent methods instead. The JSON tree once created can also be traversed and an action method (IAction iterface) called for each type of node traversed to either read its details or modify it.

The examples folder only contains a series of programs that use for library interface for the parsing, modification and stringification  of JSON back to text.For an in-depth description of the JSON specification refer to its RFC at web address https://tools.ietf.org/html/rfc8259.

Please note that this is still very much a work in progress.
