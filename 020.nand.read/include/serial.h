#ifndef __SERIAL_H__
#define __SERIAL_H__

int     serial_init   (void);
void	serial_putc   (const char);
void	serial_puts   (const char *);
int     serial_getc   (void);
int     serial_tstc   (void);

#endif	/* __SERIAL_H__ */