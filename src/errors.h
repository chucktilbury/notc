/**
 * @file errors.h
 *
 */
#ifndef _ERRORS_H_
#define _ERRORS_H_

void syntax(const char* fmt, ...);
void warning(const char* fmt, ...);
int get_num_errors();
int get_num_warnings();

#define fatal(fmt, ...) __fatal(__func__, __LINE__, (fmt), ## __VA_ARGS__)
void __fatal(const char* func, int line, const char* fmt, ...);

#endif
