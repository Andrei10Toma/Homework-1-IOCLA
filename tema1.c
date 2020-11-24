// Toma Andrei 321CB
// Tema 1 IOCLA

#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int write_stdout(const char *token, int length)
{
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

// s - sirule de caractere
// functia inverseaza un sir de caractere s
void reverse(char *s) {
	int i, length = strlen(s);
	char aux;
	for (i = 0; i < length / 2 + length % 2; i++) {
		aux = s[i];
		s[i] = s[length - i - 1];
		s[length - i - 1] = aux;
	}
}

// decimal - numarul intreg in baza zecimala
// hexnum - sir de caractere care reprezinta numarul in baza hexa
// functia converteste un numar din baza zecimala in hexa
void DecToHex(int decimal, char *hexnum) {
	int i = 0, rest;
	if (decimal > 0) {
		// algoritmul de convertire al unui numar pozitiv in hexa
		while(decimal != 0) {
			rest = decimal % 16;
			if (rest < 10) {
				rest = rest + '0';
			}
			else {
				rest = rest + 'a' - 10;
			}
			hexnum[i] = rest;
			i++;
			decimal /= 16;
		}
	}

	else {
		// alogritmul de convertire al unui numar negativ la hexa
		// se casteaza intregul negativ la unsigned int
		unsigned int u_decimal = (unsigned int) decimal;
		while(u_decimal != 0) {
			rest = u_decimal % 16;
			if (rest < 10) {
				rest = rest + '0';
			}
			else {
				rest = rest + 'a' - 10;
			}
			hexnum[i] = rest;
			i++;
			u_decimal /= 16;
		}
	}
	hexnum[i] = '\0';
	reverse(hexnum);
}

// x - un intreg fara semn
// s - sirul de caractere in care se retine numarul x sub forma de string
// functia transforma un intreg fara semn intr-un sir de caractere
void unsignedIntToString(unsigned int x, char *s) {
	int i = 0;
	do {
		// se extrage ultima cifra si se converteste in caracter
		s[i] = x % 10 + '0';
		i++;
		x /= 10;
	} while(x > 0);
	s[i] = '\0';
	reverse(s);
}

// x - un numar intreg
// s - sirul de caractere in care se retine intregul dupa convertire
// functia transforma un intreg intr-un sir de caractere
void intToString(long long int x, char *s) {
	int i = 0;
	long long int sign = x;
	// se obtine modulul numarului
	if (sign < 0) {
		x = -x;
	}
	do {
		s[i] = x % 10 + '0';
		i++;
		x /= 10;
	} while(x > 0);
	if (sign < 0) {
		s[i] = '-';
		i++;
	}
	s[i] = '\0';
	reverse(s);
}

int iocla_printf(const char *format, ...)
{
	int numar_caractere = 0;
	char s[200]; s[0] = '\0';
	va_list args;
	va_start(args, format);
	// se parcurge formatul caracter cu caracter
	for (unsigned int i = 0; i < strlen(format); i++) {
		// este gasit inceputul unui specificator
		if (format[i] == '%') {
			s[0] = '\0';
			// verificare daca este intreg
			if (format[i + 1] == 'd') {
				long long int x = va_arg(args, int);
				intToString(x, s);
				write_stdout(s, strlen(s));
				numar_caractere += strlen(s);
			}
			// verificare daca se doreste scrierea in hexa
			else if (format[i + 1] == 'x') {
				int x = va_arg(args, int);
				DecToHex(x, s);
				write_stdout(s, strlen(s));
				numar_caractere += strlen(s);
			}
			// verificare daca se doreste scrierea unui intreg fara semn
			else if (format[i + 1] == 'u') {
				unsigned int x = va_arg(args, unsigned int);
				unsignedIntToString(x, s);
				write_stdout(s, strlen(s));
				numar_caractere += strlen(s);
			}
			// verificare daca se doreste scrierea unui sir de caractere
			else if (format[i + 1] == 's') {
				strcpy(s, va_arg(args, char *));
				write_stdout(s, strlen(s));
				numar_caractere += strlen(s);
			}
			// verificare daca se doreste printarea unui caracter
			else if (format[i + 1] == 'c') {
				char c = va_arg(args, int);
				write_stdout(&c, 1);
				numar_caractere++;
			}
			// verificare daca se doreste printarea lui '%' escapat
			else if (format[i + 1] == '%') {
				write_stdout(&format[i], 1);
				numar_caractere++;
			}
			i++;
		}
		// se printeaza caracterul care nu este identificat cu un specificator 
		else {
			write_stdout(&format[i], 1);
			numar_caractere++;
		}
	}
	// se returneaza lungimea sirului printat la output
	return numar_caractere;
}