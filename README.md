# FluxOS

FluxOS is a generic UNIX operating system designed for radio comunications.
The project is foremost a learning experience. The final outcome of the system is vague.

## Technical details
About the innerworkings of the system

### Naming Conventions
These rules can be ignored if it enhances readibility. as the project progresses
it is also posible for these rules to change.

**Types:**	Each individual word starts with a upper case. *PageEntry, Line*

**Variable:**	Each individual word starts with a upper case except the first one. *pageEntry, line*

**Constants:**	All caps with underscores to seperate words. *PAGE_ENTRY, LINE*

**Function:**	The same as vars. *getPageEntry, computeLine*

**namespaces:**	completly lowercase. if needed with underscore to seperate words if needed
				however avoid this if possible. *memory*

**Variable** Private class variables should have underscore suffix. *line_*

**Variable** Generic variables should have the same name as their type.

**Variable** Variables with a large scope should have long names, variables with a small scope can have short names.

**Function** The name of the object is implicit, and should be avoided in a method name.

**Function** The terms get/set must be used where an attribute is accessed directly.

**Function** The term initialize can be used where an object or a concept is established.

**Variable** Variables representing GUI components should be suffixed by the component type name. *mainWindow, propertiesDialog*

**Variable** The prefix n should be used for variables representing a number of objects. *nPoints, nLines*

**Variable** Plural form should be used on names representing a collection of objects.

**Variable** variable used for iteration should be called i, j, k etc.

**Pointers** Naming pointers specifically should be avoided. *Line* **line*

**Enumeration constants** should not be prefixed with a common constant. *BLUE, RED, ORANGE*

**Exception classes** should be suffixed with Exception. *AccessException*

**Functions** (methods returning something) should be named after what they return and procedures (void methods) after what they do.

**General** Abbreviations in names should be avoided. *computeAverage(); not computeAvg();*

## Paging
Finish this!