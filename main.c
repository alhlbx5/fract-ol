/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalhalab <aalhalab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:38:32 by aalhalab          #+#    #+#             */
/*   Updated: 2024/06/28 00:11:38 by aalhalab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define MAX_ITER 100

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
	int             draw;
	bool            is_running;
} t_fract;

void ft_zoom(double xdelta, double ydelta, void *param);
void handleKeyPress(mlx_key_data_t keyEvent, void *fractalParams);
void draw_pixel(t_fract *fract, int x, int y, int color);
int calculate_pixel_color(t_fract *fract, int x, int y);
void iterate_pixels(t_fract *fract, int x, int y);
int calculate_iteration(t_fract *fract, int x, int y);
int calculate_color_from_iter(int iter);
int calculate_pixel_color(t_fract *fract, int x, int y);

int calculate_iteration(t_fract *fract, int x, int y) 
{
	double real = fract->x_min + (double)x / WIDTH * (fract->x_max - fract->x_min);
	double imag = fract->y_min + (double)y / HEIGHT * (fract->y_max - fract->y_min);
	double c_real = real;
	double c_imag = imag;
	int iter = 0;
	while (real * real + imag * imag <= 4 && iter < MAX_ITER) 
	{
		double temp_real = real * real - imag * imag + c_real;
		imag = 2 * real * imag + c_imag;
		real = temp_real;
		iter++;
	}
	return iter;
}

int calculate_color_from_iter(int iter) 
{
	if (iter < MAX_ITER) 
	{
		double t = (double)iter / MAX_ITER;
		int red = (int)((1 - t) * 0 + t * 255);
		int green = (int)((1 - t) * 0 + t * 0);
		int blue = (int)((1 - t) * 255 + t * 0);
		return (red << 16) | (green << 8) | blue;
	} 
	else 
		return 0;
}

int calculate_pixel_color(t_fract *fract, int x, int y) 
{
	int iter = calculate_iteration(fract, x, y);
	return calculate_color_from_iter(iter);
}

void draw_pixel(t_fract *fract, int x, int y, int color)
{
	uint8_t *pixel = &fract->img->pixels[(x + y * WIDTH) * 4];
	pixel[0] = color & 0xFF;        // Blue
	pixel[1] = (color >> 8) & 0xFF; // Green
	pixel[2] = (color >> 16) & 0xFF; // Red
	pixel[3] = 0xFF;                // Alpha
}

void iterate_pixels(t_fract *fract, int x, int y)
{
	if (y >= HEIGHT) return;
	if (x >= WIDTH)
	{
		iterate_pixels(fract, 0, y + 1);
		return;
	}
	int color = calculate_pixel_color(fract, x, y);
	draw_pixel(fract, x, y, color);
	iterate_pixels(fract, x + 1, y);
}
void ft_zoom(double deltaX, double deltaY, void *parameters)
{
	(void)deltaX;
	t_fract *fractal = (t_fract *)parameters;
	double scaleFactor = 1.1;
	double realMousePosition = fractal->x_min + fractal->x_pos * (fractal->x_max - fractal->x_min) / WIDTH;
	double imaginaryMousePosition = fractal->y_min + fractal->y_pos * (fractal->y_max - fractal->y_min) / HEIGHT;

   // printf("befor x_min: %f\n", fractal->x_min);
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
	//printf("after x_min: %f\n", fractal->x_min);
}


void handleKeyPress(mlx_key_data_t keyEvent, void *fractalParams)
{
	t_fract *fractal = (t_fract *)fractalParams;

	if (keyEvent.key == MLX_KEY_ESCAPE && keyEvent.action == MLX_PRESS)
	{
		mlx_close_window(fractal->mlx);
		mlx_terminate(fractal->mlx);
	}
	if (keyEvent.key == MLX_KEY_UP && keyEvent.action == MLX_PRESS)
		{
	   
		fractal->y_min += (fractal->y_max - fractal->y_min) * (0.01);
		fractal->y_max += (fractal->y_max - fractal->y_min) * (0.01);   
		}
	if (keyEvent.key == MLX_KEY_DOWN && keyEvent.action == MLX_PRESS)
		{
		fractal->y_min -= (fractal->y_max - fractal->y_min) * (0.01);
		fractal->y_max -= (fractal->y_max - fractal->y_min) * (0.01);
		}
	if (keyEvent.key == MLX_KEY_LEFT && keyEvent.action == MLX_PRESS)
		{
			fractal->x_min += (fractal->x_max - fractal->x_min) * (0.01);
		fractal->x_max += (fractal->x_max - fractal->x_min) * (0.01);  
		}
	if (keyEvent.key == MLX_KEY_RIGHT && keyEvent.action == MLX_PRESS)
		{
			fractal->x_min -= (fractal->x_max - fractal->x_min) * (0.01);
			fractal->x_max -= (fractal->x_max - fractal->x_min) * (0.01);  
		}

	fractal->draw = 1;
}

void update(void *param)
 {
	//mlx_t* mlx = param;
	t_fract* fract = param;
	mlx_t* mlx = fract->mlx;
	
	if (fract->draw)
	{
		iterate_pixels(fract, 0, 0);
		fract->draw = 0;
		mlx_image_to_window(mlx, fract->img, 0, 0);
	}
}

int main(void)
{
	mlx_t *mlx;
	t_fract *fract;

	fract = malloc(sizeof(t_fract)); 
	if (!fract)
	{
		fprintf(stderr, "Error: Memory allocation for fract failed\n");
		return 1; 
	}
	fract->is_running = true;
	mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true);
	if (!mlx)
	{
		fprintf(stderr, "Error: mlx_init failed\n");
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
	fract->color = 0x00FF00;
	fract->draw = 1;
	mlx_loop_hook(mlx, update, fract);
	mlx_scroll_hook(mlx, ft_zoom, fract);
	mlx_key_hook(mlx, handleKeyPress, fract);
	mlx_loop(mlx);
	mlx_delete_image(mlx, fract->img);
	free(fract);
	mlx_terminate(mlx);
	return 0;
}
