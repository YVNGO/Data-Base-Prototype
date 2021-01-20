# Simple-Data-Base

Storage of Date-Event pairs
There can be multiple events in the same date
Event can't be an empty string
It is sufficient to store only 1 unique event in unique date, database ignores duplicates

For communication with user using stdin , stdout the database understands these commands:

-adding of event:                                             "Add Date Event"

-deleting event:                                              "Del Date Event"

-deleting of all events in particular date:                   "Del Date"

-searching of events for particular date:                     "Find Date"

-printing of all events for all dates:                        "Print"

All of the commands are written in separate lines by the user
