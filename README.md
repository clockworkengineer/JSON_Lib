# JSONLib
Perform JSON  stringify/parse to/from a byte or file. It is also  
possible to customize this with the ISource and IDestination interfaces 
if required. Note: At present it will report incorrect JSON syntax 
but will not be specific about what error has occurred; this is reasoned 
to add too much overhead to  the process of parsing for the requirements 
of this library (this might change in future versions). For an in-depth 
description of the JSON specification refer to its RFC at web address
https://tools.ietf.org/html/rfc8259.
