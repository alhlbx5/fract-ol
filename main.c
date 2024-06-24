/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalhalab <aalhalab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:38:32 by aalhalab          #+#    #+#             */
/*   Updated: 2024/06/24 04:47:18 by aalhalab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "MLX42/MLX42.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct s_fract
{
	mlx_image_t		*img;
	mlx_t			*mlx;
	int				color;
	int32_t			x_pos;
	int32_t			y_pos;
	double			x_min;
	double			y_min;
	double			x_max;
	double			y_max;
	double			z_re;
	double			z_im;
	double			c_re;
	double			c_im;
	double			re;
	double			im;
	int				iter;
	int				draw;
}					t_fract;


int main(int ac, char *av[])
{
    mlx_t       *mlx;
    t_fract   *fract;

    mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true);
    if(!mlx)
        return(ft_printf("Error: mlx_init failed\n"), 1);
    fract->mlx = mlx;
    fract->img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!fract->img)
    {
        ft_printf("Error: mlx_new_image failed\n");
        return (1);
    }
    if (mlx_image_to_window(mlx, fract->img, 0, 0) == -1)
    {
        ft_printf("Error: mlx_image_to_window failed\n");
        return (mlx_close_window(mlx), 1);
    }
    mlx_scrool_hook(mlx, ft_zoom, fract);
    mlx_key_hook(mlx, ft_key, fract);
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return (0);
}
