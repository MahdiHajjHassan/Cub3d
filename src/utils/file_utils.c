// File helpers: read entire file into an array of lines, and free helpers.
#include "cub3d.h"

static void	*free_partial(char **lines, size_t count)
{
	// Free already-allocated subset when a later allocation fails
	free_lines(lines, count);
	return (NULL);
}

// Read the whole file into a growable buffer; returns buffer and length.
static int	read_file_content(const char *path, char **content_out, size_t *len_out)
{
	int		fd;
	char	buffer[4096];
	ssize_t	bytes;
	char	*content;
	size_t	cap;
	size_t	len;

	// Open file for reading bytes
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (error_msg("cannot open file"));
	cap = 8192;
	len = 0;
	content = (char *)malloc(cap);
	if (!content)
		return (close(fd), error_msg("malloc failed"));
	// Read chunks and grow buffer as needed
	while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
	{
		if (len + (size_t)bytes >= cap)
		{
			while (len + (size_t)bytes >= cap)
				cap *= 2;
			char *new_buf = (char *)malloc(cap);
			if (!new_buf)
				return (free(content), close(fd), error_msg("malloc failed"));
			size_t j = 0;
			while (j < len)
			{
				new_buf[j] = content[j];
				++j;
			}
			free(content);
			content = new_buf;
		}
		ssize_t j2 = 0;
		while (j2 < bytes)
		{
			content[len++] = buffer[j2];
			++j2;
		}
	}
	close(fd);
	if (bytes < 0)
		return (free(content), error_msg("read failed"));
	*content_out = content;
	*len_out = len;
	return (0);
}

// Allocate and copy a substring [start, start+length) into a new NUL-terminated string.
static char	*substr_dup(const char *src, size_t start, size_t length)
{
	char *out;
	size_t i;

	// Allocate buffer for substring + NUL
	out = (char *)malloc(length + 1);
	if (!out)
		return (NULL);
	i = 0;
	while (i < length)
	{
		out[i] = src[start + i];
		++i;
	}
	// Terminate the copied slice
	out[length] = '\0';
	return (out);
}

// Split the raw file content into a NULL-terminated array of lines.
static int	split_content_into_lines(const char *content, size_t len,
			char ***out_lines, size_t *out_count)
{
	char	**lines;
	size_t	line_count;
	size_t	i;
	size_t	start;
	size_t	j;

	line_count = 0;
	j = 0;
	while (j < len)
		if (content[j++] == '\n')
			++line_count;
	// Allocate array of pointers (+1 for NULL terminator)
	lines = (char **)malloc(sizeof(char *) * (line_count + 1));
	if (!lines)
		return (error_msg("malloc failed"));
	i = 0;
	start = 0;
	j = 0;
	while (j < len)
	{
		if (content[j] == '\n')
		{
			// Copy substring [start, j)
			char *line = substr_dup(content, start, j - start);
			if (!line)
				return (free_partial(lines, i), error_msg("malloc failed"));
			lines[i++] = line;
			start = j + 1;
		}
		++j;
	}
	// Handle last trailing segment if file lacks newline
	if (start < len)
	{
		char *line = substr_dup(content, start, len - start);
		if (!line)
			return (free_partial(lines, i), error_msg("malloc failed"));
		lines[i++] = line;
	}
	lines[i] = NULL;
	*out_lines = lines;
	*out_count = i;
	return (0);
}

// Public API: read all lines from file path into `out_lines` and `out_count`.
int	read_all_lines(const char *path, char ***out_lines, size_t *out_count)
{
	char	*content;
	size_t	len;

	if (!out_lines || !out_count)
		return (error_msg("internal: invalid args"));
	*out_lines = NULL;
	*out_count = 0;
	// Read the entire file to a contiguous buffer
	if (read_file_content(path, &content, &len) != 0)
		return (1);
	// Split the buffer by newlines into a string array
	if (split_content_into_lines(content, len, out_lines, out_count) != 0)
		return (free(content), 1);
	free(content);
	return (0);
}

// Free an array of `count` lines and then the array pointer.
void	free_lines(char **lines, size_t count)
{
	size_t i;

	if (!lines)
		return ;
	i = 0;
	while (i < count)
	{
		// Free each line pointer
		free(lines[i]);
		++i;
	}
	// Free the array itself
	free(lines);
}


