/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalhalab <aalhalab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:38:32 by aalhalab          #+#    #+#             */
/*   Updated: 2024/06/25 14:29:28 by aalhalab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "MLX42/MLX42.h"
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct s_fract
{
	mlx_image_t     *img;
	mlx_t           *mlx;
	int             color;
	int32_t         x_pos;
	int32_t         y_pos;
	double          x_min;
	double          y_min;
	double          x_max;
	double          y_max;
	int             iter;
	int             draw;
} t_fract;

void ft_zoom(double xdelta, double ydelta, void *param);
void handleKeyPress(mlx_key_data_t keyEvent, void *fractalParams);
void draw_fractal(t_fract *fract);

void ft_zoom(double deltaX, double deltaY, void *parameters)
{
	t_fract *fractal = (t_fract *)parameters;
	double scaleFactor = 1.1;
	double realMousePosition = fractal->x_min + fractal->x_pos * (fractal->x_max - fractal->x_min) / WIDTH;
	double imaginaryMousePosition = fractal->y_min + fractal->y_pos * (fractal->y_max - fractal->y_min) / HEIGHT;

	if (deltaY > 0)
	{
		fractal->x_min = realMousePosition + (fractal->x_min - realMousePosition) / scaleFactor;
		fractal->x_max = realMousePosition + (fractal->x_max - realMousePosition) / scaleFactor;
		fractal->y_min = imaginaryMousePosition + (fractal->y_min - imaginaryMousePosition) / scaleFactor;
		fractal->y_max = imaginaryMousePosition + (fractal->y_max - imaginaryMousePosition) / scaleFactor;
	}
	else
	{
		fractal->x_min = realMousePosition + (fractal->x_min - realMousePosition) * scaleFactor;
		fractal->x_max = realMousePosition + (fractal->x_max - realMousePosition) * scaleFactor;
		fractal->y_min = imaginaryMousePosition + (fractal->y_min - imaginaryMousePosition) * scaleFactor;
		fractal->y_max = imaginaryMousePosition + (fractal->y_max - imaginaryMousePosition) * scaleFactor;
	}

	fractal->draw = 1;
}

void handleKeyPress(mlx_key_data_t keyEvent, void *fractalParams)
{
	t_fract *fractal = (t_fract *)fractalParams;
	const int moveOffset = 10;
	const double initialXMin = -2.0, initialXMax = 2.0;
	const double initialYMin = -1.5, initialYMax = 1.5;

	if (keyEvent.key == MLX_KEY_ESCAPE && keyEvent.action == MLX_PRESS)
		mlx_close_window(fractal->mlx);
	else if (keyEvent.key == MLX_KEY_UP && keyEvent.action == MLX_PRESS)
		fractal->y_pos -= moveOffset;
	else if (keyEvent.key == MLX_KEY_DOWN && keyEvent.action == MLX_PRESS)
		fractal->y_pos += moveOffset;
	else if (keyEvent.key == MLX_KEY_LEFT && keyEvent.action == MLX_PRESS)
		fractal->x_pos -= moveOffset;
	else if (keyEvent.key == MLX_KEY_RIGHT && keyEvent.action == MLX_PRESS)
		fractal->x_pos += moveOffset;
	else if (keyEvent.key == MLX_KEY_R && keyEvent.action == MLX_PRESS)
	{
		fractal->x_min = initialXMin;
		fractal->x_max = initialXMax;
		fractal->y_min = initialYMin;
		fractal->y_max = initialYMax;
		fractal->x_pos = WIDTH / 2;
		fractal->y_pos = HEIGHT / 2;
	}

	fractal->draw = 1;
}

int main(void)
{
	mlx_t *mlx;
	t_fract *fract;

	mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true);
	if (!mlx)
	{
		fprintf(stderr, "Error: mlx_init failed\n");
		return 1;
	}
	fract = (t_fract *)malloc(sizeof(t_fract));
	if (!fract)
	{
		fprintf(stderr, "Error: malloc failed\n");
		return 1;
	}
	fract->mlx = mlx;
	fract->img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!fract->img)
	{
		fprintf(stderr, "Error: mlx_new_image failed\n");
		return 1;
	}
	if (mlx_image_to_window(mlx, fract->img, 0, 0) == -1)
	{
		fprintf(stderr, "Error: mlx_image_to_window failed\n");
		return 1;
	}
	fract->x_min = -2.0;
	fract->x_max = 2.0;
	fract->y_min = -1.5;
	fract->y_max = 1.5;
	fract->x_pos = WIDTH / 2;
	fract->y_pos = HEIGHT / 2;
	fract->iter = 50;
	fract->color = 0x00FF00;
	fract->draw = 1;

	mlx_scroll_hook(mlx, ft_zoom, fract);
	mlx_key_hook(mlx, handleKeyPress, fract);

	while (1)
	{
		mlx_loop(mlx);
		if (fract->draw)
		{
			draw_fractal(fract);
			fract->draw = 0; 
		}
	}
free(fract); 
}
