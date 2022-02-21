# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <string.h>


typedef struct s_zone
{
	int width;
	int height;
	char bc;
}	t_zone;

typedef struct s_draw
{
	char type;
	char c;
	float x;
	float y;
	float h;
	float w;
}	t_draw;

int ft_strlen(char *str)
{
	int i = 0;
	while(str[i])
		++i;
	return i;
}

int ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
	return 1;
}

void ft_puttab(char **tab)
{
	int i = 0;
	while (tab[i])
	{
		ft_putstr(tab[i]);
		write(1, "\n", 1);
		i++;
	}
}

int ft_freetab(char **tab)
{
	int i = 0;
	while(tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return 1;
}

static int init_paint(char **paint, t_zone zone)
{
	int i;
	
	if (!paint)
		return 1;
	i = 0;
	while(i < zone.height)
	{
		if (!(paint[i] = calloc(1, zone.width +1)))
			return 1;
		memset(paint[i], zone.bc, zone.width);
		++i;
	}
	paint[i] = 0;
	return 0;
}	

static int isinrectangle(float x, float y, t_draw draw)
{
	float xbr;
	float ybr;

	xbr = draw.x + draw.w;
	ybr = draw.y + draw.h;
	if(x >= draw.x && x <= xbr && y >= draw.y && y <= ybr)
	{
		if (x - draw.x < 1.0 || xbr - x < 1.0 || y - draw.y < 1.0 || ybr - y < 1.0)
			return 0;
		return 1;
	}
	return -1;
}

static int drawrectangle(t_zone zone, t_draw draw, char **paint)
{
	int i;
	int j;
	int k;
	i = 0;
	while( i < zone.height)
	{
		j = 0;
		while (j < zone.width)
		{
			k = isinrectangle(j, i, draw);
			if(k == 0 || (k == 1 &&draw.type == 'R'))
			paint[i][j] = draw.c;
			++j;
		}
		++i;
	}
	return 0;
}

static int drawing(FILE *stream, t_zone zone)
{
	int ret;
	char **paint;
	t_draw draw;
	char n;

	paint = (char **)malloc(sizeof(char *) * (zone.height + 1));
	if (init_paint(paint, zone) == 1)
		return (ft_freetab(paint));
	draw.type = 0;
	while((ret = fscanf(stream, "%c%c %f %f %f %f %c", &n, &draw.type, &draw.x, &draw.y,
	&draw.w, &draw.h, &draw.c)) == 7)
	{
		if((draw.type != 'R' && draw.type != 'r' )|| draw.h <= 0.0 ||
			draw.w <= 0.0 || n != '\n' || draw.c == 0 || draw.c == '\n')
		return ft_freetab(paint);
		drawrectangle(zone, draw, paint);
		draw.type = 0;
	}
	if (draw.type != 0 && ret != -1)
		return ft_freetab(paint);
	ft_puttab(paint);
	ft_freetab(paint);
	return 0;		
}

static int get_info(FILE *stream, t_zone *zone)
{
	int ret;

	ret = fscanf(stream, "%d %d %c", &zone->width, &zone->height, &zone->bc);
	if(zone->height <= 0 || zone->height > 300 || zone->width <= 0 || zone->width > 300
	|| zone->bc == 0 || zone->bc == '\n')
		return 1;
	return 0;
}

int main(int ac, char **av)
{
	FILE *stream;
	t_zone zone;

	if (ac != 2)
		return (ft_putstr("Error: argument\n"));
	if(!(stream = fopen(av[1], "r")))
		return (ft_putstr("Error: Operation file corrupted\n"));
	if (get_info(stream, &zone) == 1)
		return (ft_putstr("Error: Operation file corrupted\n"));
	if (drawing(stream, zone) == 1)
		return (ft_putstr("Error: Operation file corrupted\n"));
	fclose(stream);
	return (0);
}

