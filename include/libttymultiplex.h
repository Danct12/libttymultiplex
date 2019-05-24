// Copyright (c) 2018 Daniel Abrecht
// SPDX-License-Identifier: AGPL-3.0-or-later

#ifndef LIBTTYMULTIPLEX_H
#define LIBTTYMULTIPLEX_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef TYM_EXPORT
#ifdef TYM_BUILD
#define TYM_EXPORT __attribute__((visibility ("default")))
#else
#define TYM_EXPORT
#endif
#endif

#define TYM_I_CONCAT_SUB(A,B) A ## B
#define TYM_I_CONCAT(A,B) TYM_I_CONCAT_SUB(A,B)
#define TYM_I_UNPACK(...) __VA_ARGS__
#define TYM_I_DOKUMENTATION(...) /** __VA_ARGS__ */

#ifdef __cplusplus
extern "C" {
#endif

#define TYM_I_SPECIAL_KEYS \
  X(ENTER    , '\n'  ) \
  X(HOME     , '\r'  ) \
  X(BACKSPACE, '\b'  ) \
  X(TAB      , '\t'  ) \
  X(ESCAPE   , '\x1B') \
  X(DELETE   , '\x7F') \
  X(UP       , 0x100 ) \
  X(DOWN     , 0x101 ) \
  X(RIGHT    , 0x102 ) \
  X(LEFT     , 0x103 ) \
  X(END      , 0x104 ) \
  X(PAGE_UP  , 0x105 ) \
  X(PAGE_DOWN, 0x106 ) \

/**
 * These are constants representing special keys.
 * These constants are not indeces for tym_special_key_list,
 * but can be used with tym_pane_send_key and tym_pane_send_keys.
 */
enum tym_special_key {
#define X(ID, VAL) TYM_KEY_ ## ID = VAL,
TYM_I_SPECIAL_KEYS
#undef X
};

/** Informations about supported special keys. */
struct tym_special_key_name {
  /** The key constant */
  enum tym_special_key key;
  /** The key name, null terminated */
  const char* name;
  /** The length of the key name */
  size_t name_length;
};

/** A list of all special keys. */
extern const struct tym_special_key_name tym_special_key_list[];
/** The length of the list of special keys. */
extern const size_t tym_special_key_count;

/** All possible mouse button state */
enum tym_button {
  TYM_BUTTON_LEFT_PRESSED,
  TYM_BUTTON_MIDDLE_PRESSED,
  TYM_BUTTON_RIGHT_PRESSED,
  TYM_BUTTON_RELEASED,
};

#define TYM_I_UNIT_TYPE__INTEGER(F) F(INTEGER, long  , integer)
#define TYM_I_UNIT_TYPE__REAL(F)    F(REAL   , double, real   )

#define TYM_I_UNIT_TYPE_LIST(F) \
  TYM_I_UNIT_TYPE__INTEGER(F) \
  TYM_I_UNIT_TYPE__REAL(F)

#define TYM_I_UNIT_LOOKUP(N,F) TYM_I_CONCAT(TYM_I_UNIT_TYPE__, N)(F)

#define TYM_I_POSITION_TYPE__CHARFIELD(F) F(CHARFIELD, INTEGER, (The position from the top left corner in characters))
#define TYM_I_POSITION_TYPE__RATIO(F)     F(RATIO    , REAL   , (The position as a factor of the length along a tym_axis. 0 -> top left, 1 -> bottom right.))

#define TYM_I_POSITION_TYPE_LIST(F) \
  TYM_I_POSITION_TYPE__CHARFIELD(F) \
  TYM_I_POSITION_TYPE__RATIO(F)

#define TYM_I_POSITION_LOOKUP(N,F) TYM_I_CONCAT(TYM_I_POSITION_TYPE__, N)(F)

/**
 * Different types of positions for tym_position, which differ in what they specify the position in relation to.
 * There is always only one valid #tym_unit_type for any given #tym_position_type.
 */
enum tym_position_type {
  /** The position type was never set */
  TYM_P_UNSET,
#define X(PT, U, DOC) TYM_I_DOKUMENTATION DOC TYM_P_ ## PT,
  TYM_I_POSITION_TYPE_LIST(X)
#undef X
  /** The number of position types. */
  TYM_P_COUNT
};

/** The unit types for tym_unit. */
enum tym_unit_type {
  /** The unit type was never set */
  TYM_U_UNSET,
#define X(U, T, N) TYM_U_ ## U,
  TYM_I_UNIT_TYPE_LIST(X)
#undef X
  /** The number of position types */
  TYM_U_COUNT
};

/** The axis of our space, one for each dimension */
enum tym_axis {
  TYM_AXIS_HORIZONTAL,
  TYM_AXIS_VERTICAL,
  TYM_AXIS_COUNT
};

/** Every direction, two for each #tym_axis */
enum tym_direction {
  TYM_LEFT,
  TYM_RIGHT,
  TYM_TOP,
  TYM_BOTTOM,
  TYM_DIRECTION_COUNT
};

/** The edges of the tym_*_position_rectangle types. */
enum tym_rectangle_edge {
  TYM_RECT_TOP_LEFT,
  TYM_RECT_BOTTOM_RIGHT,
  TYM_RECT_EDGE_COUNT
};

/** Flags for #tym_pane_set_flag */
enum tym_flag {
  TYM_PF_FOCUS, //!< Try to set the focus on a pane
  TYM_PF_DISALLOW_FOCUS, //!< Disalow setting the focus to a pane. Removes the focus from a pane if it already has it.
};

/**
 * TYM_PANE_FOCUS always refers to the pane which is currently in focus.
 **/
#define TYM_PANE_FOCUS 1

/**
 * A value of the type specified by #tym_unit_type.
 */
struct tym_unit {
  /** The type specifies which value has to be used. */
  enum tym_unit_type type;
  union {
#define X(U, T, N) T N;
  TYM_I_UNIT_TYPE_LIST(X)
#undef X
  } value;
};

/* A position specified as an #tym_position_type. */
struct tym_position {
  /** The position type of the this position, see #tym_position_type. */
  enum tym_position_type type;
  /** All coordinates, one for each #tym_axis. */
  struct tym_unit axis[TYM_AXIS_COUNT];
};

#define TYM_I_POSITION_SPECIALISATION(X, DOKUMENTATION) \
  TYM_I_DOKUMENTATION DOKUMENTATION \
  struct tym_ ## X ## _position { \
    /** The position specified using every possible type. */ \
    struct tym_position type[TYM_P_COUNT]; \
  }; \
  /** tym_ ## X ## _position_rectangle specifies a rectangular region by specifying it's top left and bottom right corner using tym_ ## X ## _position. <br/><br/>To access this structure, consider using the #TYM_RECT_POS_REF macro. */ \
  struct tym_ ## X ## _position_rectangle { \
    /** The position of the top left (TYM_RECT_TOP_LEFT) and bottom right (TYM_RECT_BOTTOM_RIGHT) corners. */ \
    struct tym_ ## X ## _position edge[TYM_RECT_EDGE_COUNT]; \
  };

TYM_I_POSITION_SPECIALISATION(super, (
  The final position of a tym_super_position is the sum of all tym_position.
  There is one for each position type. This allows to specify a position in a way
  that is agnostic to screen size and other units whose size may differ in different
  situations. This isn not enough for implementing responsive designs, though.
  <br/><br/>
  To access this structure, consider using the #TYM_POS_REF macro.
))

TYM_I_POSITION_SPECIALISATION(absolute, (
  A tym_absolute_position contains the absolute computed position in every possible position type.
  <br/><br/>
  To access this structure, consider using the #TYM_POS_REF macro.
 ))

#undef TYM_I_POSITION_SPECIALISATION

#define TYM_I_UNIT_NAME(U, T, N) N
#define TYM_I_PT_UNIT_NAME(PT, U, DOC) TYM_I_UNIT_LOOKUP(U, TYM_I_UNIT_NAME)

/**
 * For simpler usage of the specialisations of tym_position,
 * currently, these are tym_super_position and tym_absolute_position.
 * This macro automatically selects the correct field for the given position type and axis.
 * It is allowed to reference, set and read the selected field. The type of the field depends on the position type.
 * 
 * \param POSITION A specialisations of tym_position such as tym_super_position or tym_absolute_position.
 * \param POSITION_TYPE One of the fields of #tym_position_type, but without TYM_P_ prefix. Currently, only CHARFIELD and RATIO are possible values.
 * \param AXIS One of the values of #tym_axis.
 **/
#define TYM_POS_REF(POSITION, POSITION_TYPE, AXIS) \
  (POSITION).type[TYM_P_ ## POSITION_TYPE].axis[(AXIS)].value.TYM_I_POSITION_LOOKUP(POSITION_TYPE, TYM_I_PT_UNIT_NAME)

/**
 * For simpler usage of the tym_*_position_rectangle objects,
 * currently, these are tym_super_position_rectangle and tym_absolute_position_rectangle.
 * This macro automatically selects the correct field for the given position type and edge of the rectangle.
 * It is allowed to reference, set and read the selected field. The type of the field depends on the position type.
 * 
 * \param RECT One of the tym_*_position_rectangle objects such as tym_super_position_rectangle or tym_absolute_position_rectangle.
 * \param POSITION_TYPE One of the fields of #tym_position_type, but without TYM_P_ prefix. Currently, only CHARFIELD and RATIO are possible values.
 * \param POSITION One of the values of #tym_direction.
 **/
#define TYM_RECT_POS_REF(RECT, POSITION_TYPE, POSITION) \
  TYM_POS_REF((RECT).edge[(POSITION) % TYM_RECT_EDGE_COUNT], POSITION_TYPE, (POSITION) / TYM_RECT_EDGE_COUNT)

/**
 * The size of a tym_*_position_rectangle object in one of the position types along one of the axis directions.
 * This is only really usful for tym_absolute_position_rectangle opjects.
 * 
 * \param RECT One of the tym_*_position_rectangle objects such as tym_super_position_rectangle or tym_absolute_position_rectangle.
 * \param POSITION_TYPE One of the fields of #tym_position_type, but without TYM_P_ prefix. Currently, only CHARFIELD and RATIO are possible values.
 * \param AXIS One of the values of #tym_axis.
 **/
#define TYM_RECT_SIZE(RECT, POSITION_TYPE, AXIS) \
  (TYM_POS_REF((RECT).edge[TYM_RECT_BOTTOM_RIGHT], POSITION_TYPE, (AXIS)) - TYM_POS_REF((RECT).edge[TYM_RECT_TOP_LEFT], POSITION_TYPE, (AXIS)))

/**
 * Type for callback function for #tym_register_resize_handler and #tym_unregister_resize_handler.
 * This callback is called if a panes computed position and/or size changes or has to be recomputed.
 * 
 * \param ptr The pointer passed to tym_register_resize_handler.
 * \param pane The pane which was resized or changed position.
 * \param input The position & size of the pane as specified, see tym_super_position and tym_super_position_rectangle for details.
 * \param computed The computed & absolute position & size, see tym_absolute_position and tym_absolute_position_rectangle for details.
 */
typedef void(*tym_resize_handler_t)(void* ptr, int pane, const struct tym_super_position_rectangle* input, const struct tym_absolute_position_rectangle* computed );

/** Mapping from #tym_position_type to #tym_unit_type. */
extern const enum tym_unit_type tym_positon_unit_map[];

TYM_EXPORT int tym_init(void);
TYM_EXPORT int tym_shutdown(void);
TYM_EXPORT int tym_pane_create(const struct tym_super_position_rectangle*restrict super_position);
TYM_EXPORT int tym_pane_destroy(int pane);
TYM_EXPORT int tym_pane_resize(int pane, const struct tym_super_position_rectangle*restrict super_position);
TYM_EXPORT int tym_pane_reset(int pane);
TYM_EXPORT int tym_register_resize_handler( int pane, void* ptr, tym_resize_handler_t handler );
TYM_EXPORT int tym_unregister_resize_handler( int pane, void* ptr, tym_resize_handler_t handler );
TYM_EXPORT int tym_pane_set_flag(int pane, enum tym_flag flag, bool status);
TYM_EXPORT int tym_pane_get_flag(int pane, enum tym_flag flag);
TYM_EXPORT int tym_pane_set_env(int pane);
TYM_EXPORT int tym_pane_get_slavefd(int pane);

TYM_EXPORT int tym_pane_send_key(int pane, int_least16_t key);
TYM_EXPORT int tym_pane_send_keys(int pane, size_t count, const int_least16_t keys[count]);
TYM_EXPORT int tym_pane_send_special_key_by_name(int pane, const char* key_name);
TYM_EXPORT int tym_pane_type(int pane, size_t count, const char keys[count]);
TYM_EXPORT int tym_pane_send_mouse_event(int pane, enum tym_button button, const struct tym_super_position*restrict super_position);

#ifdef __cplusplus
}
#endif

#endif
