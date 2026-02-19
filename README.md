# Project X README

Student 1: Michael Nguyen (925426358)

Student 2: Nicholas Do (924737022)

## Project Status
This the current status of the project: Fully completed and functional with full coverage (aside from a mysterious bracket)

## Known Issues
The project fails to function properly in: None that we know of

## Code References
* [Expat Functions](https://libexpat.github.io/doc/api/latest/#XML_Parse)

## Generative AI Use
I used ChatGPT for Prompt 1

### Prompt 1
#include "XMLReader.h" #include <expat.h> #include <queue> struct CXMLReader::SImplementation{ }; CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){ } CXMLReader::~CXMLReader(){ } bool CXMLReader::End() const{ } bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){ } Can you help me understand this
### Response 1
Yep — this looks confusing at first glance because it’s basically a skeleton with most of the logic missing. Let’s walk through what you’re looking at and why it’s structured this way.

Big picture: what is this?

This is the start of a C++ XML reader class that likely uses Expat, a low-level, event-driven XML parsing library.

Nothing here actually does anything yet — it’s scaffolding.

Line by line, concept by concept
1. Includes
#include "XMLReader.h"
#include <expat.h>
#include <queue>


"XMLReader.h"
The header where CXMLReader, SXMLEntity, etc. are declared.

<expat.h>
Expat is a SAX-style XML parser:

You don’t load the whole XML tree

You get callbacks like:

start element

end element

text (CDATA)

<queue>
Strong hint that parsed XML entities will be queued as they’re read.

2. The mysterious struct
struct CXMLReader::SImplementation{
};


This is the PIMPL pattern (Pointer to IMPLementation).

Why it exists:

Hide implementation details from the header

Reduce compile times

Keep the public interface clean

In the header you probably have:

class CXMLReader {
    struct SImplementation;
    std::unique_ptr<SImplementation> Impl;
};


Eventually, SImplementation will hold things like:

XML_Parser

queues of parsed entities

parsing state

flags like “end of document reached”

Right now it’s empty because the logic hasn’t been written yet.

3. Constructor
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src){
}


What this should do (but doesn’t yet):

Store the data source (file, stream, socket, etc.)

Create an XML_Parser

Register Expat callbacks

Initialize internal state

CDataSource is probably an abstraction over input (file, memory buffer, etc.).

4. Destructor
CXMLReader::~CXMLReader(){
}


Eventually responsible for:

Freeing the Expat parser (XML_ParserFree)

Cleaning up any allocated memory

Letting smart pointers clean themselves up

Empty = not implemented yet.

5. End()
bool CXMLReader::End() const{
}


This should answer:

“Have we reached the end of the XML stream?”

Typical behavior:

Return true if:

No more entities are available and

The parser has finished reading input

Right now: no return statement → undefined behavior (this won’t even compile).

6. ReadEntity()
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
}


This is the core method.

Likely intent:

Pull the next parsed XML “entity” from an internal queue

Fill entity with:

element start

element end

text / CDATA

Optionally skip CDATA if skipcdata == true

Return value probably means:

true → entity successfully read

false → no more entities / end of document

Again: totally unimplemented.

Why this feels incomplete (because it is)

This looks like:

A homework assignment

A starter framework

Or part of a larger project where you are expected to fill in the logic

As written:

It won’t compile

It doesn’t parse anything

It only shows design intent

What you should understand so far

This class is meant to be an event-based XML reader

Expat will push events → your callbacks → stored in a queue

ReadEntity() pulls events out one at a time

SImplementation hides all the ugly details

If you want, I can:

Walk you through how Expat parsing actually works

Help you design SXMLEntity

Show a minimal working implementation

Explain this in the context of your assignment or project

Just tell me what level you’re aiming for 🙂

### Changes 1
Helped me understand the goal of and what the functions do in XMLReader

### Prompt 2
in a makefile how do I make a generic rule for compilation

### Response 2
In a Makefile, you usually create a generic (pattern) rule to compile source files into object files. This avoids writing one rule per file.
Basic generic compilation rule (C)
CC      := gcc
CFLAGS  := -Wall -Wextra -O2

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

How this works:


%.o: %.c → any .o depends on the matching .c


$< → the first prerequisite (the .c file)


$@ → the target (the .o file)


Example usage
OBJS = main.o util.o math.o

myprog: $(OBJS)
	$(CC) $(OBJS) -o myprog

Make will automatically use the generic rule to build each .o.

C++ version
CXX     := g++
CXXFLAGS := -Wall -Wextra -O2

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


Using built-in rules (often enough)
Make already has implicit rules for common languages, so this may work without writing your own rule:
CFLAGS += -Wall -Wextra -O2

Then just list your objects and targets.

Generic rule with headers (optional improvement)
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

⚠️ This only works if every .c has a matching .h. For real projects, use dependency generation:
CFLAGS += -MMD -MP
-include $(OBJS:.o=.d)


Summary


Use pattern rules: %.o: %.c


Use automatic variables: $@, $<


Let Make’s implicit rules work when possible


If you want, tell me:


C, C++, or another language


single directory or subdirectories


simple project or large codebase


I can tailor the Makefile exactly to your setup.

### Changes 2
Changed some of the flags for what I needed.