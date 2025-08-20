// Lightweight string helpers used by the parser; avoid stdlib extras.
#include "cub3d.h"

int	is_space(int c)
{
	// C-style whitespace detection
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

int	is_digit(int c)
{
	// ASCII digit check
	return (c >= '0' && c <= '9');
}

size_t	skip_spaces(const char *s, size_t i)
{
	// Advance index past any whitespace
	while (s[i] && is_space((unsigned char)s[i]))
		i++;
	return (i);
}

size_t	skip_digits(const char *s, size_t i)
{
	// Advance index past consecutive digits
	while (s[i] && is_digit((unsigned char)s[i]))
		i++;
	return (i);
}

char	*str_trim_spaces(const char *s)
{
	size_t start;
	size_t end;
	char *out;
	size_t i;

	if (!s)
		return (NULL);
	// Find first non-space
	start = 0;
	while (s[start] && is_space((unsigned char)s[start]))
		start++;
	// Find end then step back over trailing spaces
	end = start;
	while (s[end])
		end++;
	while (end > start && is_space((unsigned char)s[end - 1]))
		end--;
	// Allocate and copy trimmed slice
	out = (char *)malloc(end - start + 1);
	if (!out)
		return (NULL);
	for (i = 0; start + i < end; )
	{
		out[i] = s[start + i];
		i++;
	}
	out[i] = '\0';
	return (out);
}

int	starts_with(const char *s, const char *prefix)
{
	size_t i;

	if (!s || !prefix)
		return (0);
	i = 0;
	while (prefix[i])
	{
		if (s[i] != prefix[i])
			return (0);
		i++;
	}
	// All prefix characters matched
	return (1);
}


