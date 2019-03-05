// Copyright (c) 2018 Daniel Abrecht
// SPDX-License-Identifier: AGPL-3.0-or-later

#ifndef TYM_INTERNAL_CHARSET_H
#define TYM_INTERNAL_CHARSET_H

enum tym_i_charset_type {
  TYM_I_CHARSET_USASCII,
  TYM_I_CHARSET_DEC_SPECIAL_CHARACTER_AND_LINE_DRAWING_SET,
  TYM_I_CHARSET_UK,
  TYM_I_CHARSET_DUTCH,
  TYM_I_CHARSET_FINNISH,
  TYM_I_CHARSET_FRENCH,
  TYM_I_CHARSET_FRENCH_CANADIAN,
  TYM_I_CHARSET_GERMAN,
  TYM_I_CHARSET_ITALIAN,
  TYM_I_CHARSET_NORWEGIAN_DANISH,
  TYM_I_CHARSET_SPANISH,
  TYM_I_CHARSET_SWEDISH,
  TYM_I_CHARSET_SWISS,
  TYM_I_CHARSET_GENERIC_COUNT, /* ^^^ These ones have a generic translation table */
  TYM_I_CHARSET_DEFAULT = 0,
};

enum charset_selection {
  TYM_I_CHARSET_SELECTION_GL_MASK = 0x000FF,
  TYM_I_CHARSET_SELECTION_GL_G1   = 0x00000,
  TYM_I_CHARSET_SELECTION_GL_G2   = 0x00001,
  TYM_I_CHARSET_SELECTION_GL_G3   = 0x00002,
  TYM_I_CHARSET_SELECTION_GL_G4   = 0x00003,
  TYM_I_CHARSET_SELECTION_GR_MASK = 0x0FF00,
  TYM_I_CHARSET_SELECTION_GR_G1   = 0x00000,
  TYM_I_CHARSET_SELECTION_GR_G2   = 0x00100,
  TYM_I_CHARSET_SELECTION_GR_G3   = 0x00200,
  TYM_I_CHARSET_SELECTION_GR_G4   = 0x00300,
  TYM_I_CHARSET_SELECTION_UTF8    = 0x10000,
};

enum {
  TYM_I_TRANSLATION_TABLE_SIZE = 96,
  TYM_I_TRANSLATION_TABLE_LARGEST_UTF8_CHARACTER_SEQUENCE = 4,
};

struct translation_table {
  char table[TYM_I_TRANSLATION_TABLE_SIZE][TYM_I_TRANSLATION_TABLE_LARGEST_UTF8_CHARACTER_SEQUENCE];
};

extern const struct translation_table tym_i_translation_table[TYM_I_CHARSET_GENERIC_COUNT];

#endif
