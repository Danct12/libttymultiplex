// Copyright (c) 2018 Daniel Abrecht
// SPDX-License-Identifier: AGPL-3.0-or-later

#ifndef TYM_INTERNAL_MAIN_H
#define TYM_INTERNAL_MAIN_H

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <pty.h>
#include <libttymultiplex.h>

/** \file */

/**
 * The main loop has an array of file descriptors to watch.
 * These are some special file descriptors the main loop is watching.
 * These constants are an offset in that list. All entries >= SPF_COUNT
 * are pseudo terminal master (ptm) file descriptors from panes.
 */
enum tym_i_special_tym_i_poll_fds {
  SPF_POLLCTLFD, //!< This file descriptor is used to add & remove new file descriptors.
  SPF_SIGNALFD, //!< This file descriptor handles signals
  SPF_TERMINPUT, //!< This file descriptor handles input from the underlying terminal
  SPF_COUNT //!< The number of special file descriptors
};

/** The current run state */
enum tym_i_init_state {
  INIT_STATE_NOINIT, //!< The library is not initialised or already shut down.
  INIT_STATE_INITIALISED, //!< The library is initialised & running.
  INIT_STATE_FREEZE_IN_PROGRESS, //!< The library is currently freezing (stopping the main thread).
  INIT_STATE_FROZEN, //!< The library is currently inactive. In this state, fork can be safely called.
  INIT_STATE_SHUTDOWN_IN_PROGRESS, //!< The library is currently shutting down and doing cleanup stuff.
  INIT_STATE_SHUTDOWN = INIT_STATE_NOINIT //!< The library is not initialised or already shut down
};

/** Some action for the main loop to do */
enum tym_i_poll_ctl_type {
  TYM_PC_ADD, //!< Add a file dexcriptor
  TYM_PC_REMOVE, //!< remove a file descriptor
  TYM_PC_FREEZE, //!< exit main loop temporarely
};

/** A structure containing a command & parameters for the main loop */
struct tym_i_poll_ctl {
  /** Some action for the main loop to do. Also determines the union member of data to be used. */
  enum tym_i_poll_ctl_type action;
  /** data for the action */
  union {
    struct {
      int fd;
    } add;
    struct {
      int fd;
    } remove;
  } data;
};

/** The current run state */
extern enum tym_i_init_state tym_i_binit;
/** The number of file descripors watched by the main loop. */
extern size_t tym_i_poll_fdn;
/** The poll descriptor. */
extern struct pollfd* tym_i_poll_fds;
/** The file descriptor of the tty. Probably fd 0. */
extern int tym_i_tty;
/** The signal file descriptor */
extern int tym_i_sfd;
/** The two fie descriptors of the pipe used to send commands to the main loop */
extern int tym_i_pollctl[2];
/** The thread of the main loop */
extern pthread_t tym_i_main_loop;
/** The size of the screen. */
extern struct tym_absolute_position_rectangle tym_i_bounds;
/** Attributes for tym_i_lock */
extern pthread_mutexattr_t tym_i_lock_attr;
/**
 * The lock which makes sure only one thread at a time can access any of the tym_i_* functions.
 * This is a reentrant mutex.
 */
extern pthread_mutex_t tym_i_lock;

void* tym_i_main(void* ptr);
int tym_i_pollfd_add_sub(struct pollfd pfd);
int tym_i_pollfd_add(int fd);
int tym_i_pollfd_remove(int fd);
int tym_i_request_freeze(void);

#ifdef __GNUC__
void tym_i_debug(const char* format, ...) __attribute__((format(printf, 1, 2)));
#else
void tym_i_debug(const char* format, ...);
#endif

#endif
