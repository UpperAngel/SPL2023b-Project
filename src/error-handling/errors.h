#ifndef __ERRORS_H
#define __ERRORS_H
#include <stdio.h>

 enum Error {
    NoError,
    ExcessCharactersAtStartMacro,
    ExcessCharactersAtEndMacro,
    InvalidMacroName,
    InvalidCharacterInName,
    FirstCharacterNotLetter,
    ReservedKeywordName,
    NameExceedsMaxLength,
    FailedToAllocateMemory,
    InvalidFormat,
    ExcessCharactersInDataLine,
    MissingStartOfStringSign,
    MissingEndOfStringSign,
    InvalidCharInString,
    LineLimitExceeded,
    CommaAtStart,
    ConsecutiveCommas,
    MissingComma,
    EndsWithComma,
    InvalidCharInDataDirective,
    IntegerOverflow,
    MultipleSymbolDefinition,
    NonWhiteCharsBeforeQuote,
    NonWhiteCharsAfterClosingQuote,
    SymbolMustStartWithLetter,
    SymbolMustEndWithColon,
    InvalidCharInSymbol,
    UnknownOperationName,
    InvalidNumberOfOperands,
    ExcessCharactersInInstruction,
    UndefinedOperandsNames,
    InvalidOperandAddressing,
    OnlySymbolsAllowed,
    SymbolIsReservedWord,
    EmptySymbolDefinition,
    RedefinitionOfSymbolType,
    RedefinitionOfExternSymbol
};

extern const char *errorMessages[];

void handle_error(enum Error error, int line_number);

#endif
