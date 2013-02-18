/*
 * tga.c -- environment mapping demo
 * last modification: jan. 20, 2007
 *
 * Copyright (c) 2005-2007 David HENRY
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * TGA Texture loading
 */

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


//#include <GL/glut.h>
//#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* OpenGL texture info */
struct gl_texture
{
  GLsizei width;
  GLsizei height;

  GLenum format;
  GLint	internalFormat;
  GLuint id;

  GLubyte *texels;
};


#pragma pack(push, 1)
/* TGA header */
struct tga_header
{
  GLubyte id_lenght;          /* size of image id */
  GLubyte colormap_type;      /* 1 is has a colormap */
  GLubyte image_type;         /* compression type */

  short	cm_first_entry;       /* colormap origin */
  short	cm_length;            /* colormap length */
  GLubyte cm_size;            /* colormap size */

  short	x_origin;             /* bottom left x coord origin */
  short	y_origin;             /* bottom left y coord origin */

  short	width;                /* picture width (in pixels) */
  short	height;               /* picture height (in pixels) */

  GLubyte pixel_depth;        /* bits per pixel: 8, 16, 24 or 32 */
  GLubyte image_descriptor;   /* 24 bits = 0x00; 32 bits = 0x80 */
};
#pragma pack(pop)


static void
get_texture_info (struct tga_header *header, struct gl_texture *texinfo)
{
  texinfo->width = header->width;
  texinfo->height = header->height;

  switch (header->image_type)
    {
    case 3:  /* Grayscale 8 bits */
    case 11: /* Grayscale 8 bits (RLE) */
      {
	if (header->pixel_depth == 8)
	  {
	    texinfo->format = GL_LUMINANCE;
	    texinfo->internalFormat = 1;
	  }
	else /* 16 bits */
	  {
	    texinfo->format = GL_LUMINANCE_ALPHA;
	    texinfo->internalFormat = 2;
	  }

	break;
      }

    case 1:  /* 8 bits color index */
    case 2:  /* BGR 16-24-32 bits */
    case 9:  /* 8 bits color index (RLE) */
    case 10: /* BGR 16-24-32 bits (RLE) */
      {
	/* 8 bits and 16 bits images will be converted to 24 bits */
	if (header->pixel_depth <= 24)
	  {
	    texinfo->format = GL_RGB;
	    texinfo->internalFormat = 3;
	  }
	else /* 32 bits */
	  {
	    texinfo->format = GL_RGBA;
	    texinfo->internalFormat = 4;
	  }

	break;
      }
    }
}


static void
read_tga_8bits (GLubyte *data, GLubyte *colormap, struct gl_texture *texinfo)
{
  int i;
  GLubyte color;

  for (i = 0; i < texinfo->width * texinfo->height; ++i)
    {
      /* Read index color byte */
      color = data[i];

      /* Convert to RGB 24 bits */
      texinfo->texels[(i * 3) + 2] = colormap[(color * 3) + 0];
      texinfo->texels[(i * 3) + 1] = colormap[(color * 3) + 1];
      texinfo->texels[(i * 3) + 0] = colormap[(color * 3) + 2];
    }
}


static void
read_tga_16bits (GLubyte *data, struct gl_texture *texinfo)
{
  int i, j = 0;
  unsigned short color;

  for (i = 0; i < texinfo->width * texinfo->height; ++i, j += 2)
    {
      /* Read color word */
      color = data[j] + (data[j + 1] << 8);

      /* Convert BGR to RGB */
      texinfo->texels[(i * 3) + 0] = (GLubyte)(((color & 0x7C00) >> 10) << 3);
      texinfo->texels[(i * 3) + 1] = (GLubyte)(((color & 0x03E0) >>  5) << 3);
      texinfo->texels[(i * 3) + 2] = (GLubyte)(((color & 0x001F) >>  0) << 3);
    }
}


static void
read_tga_24bits (GLubyte *data, struct gl_texture *texinfo)
{
  int i, j = 0;

  for (i = 0; i < texinfo->width * texinfo->height; ++i, j += 3)
    {
      /* Read and convert BGR to RGB */
      texinfo->texels[(i * 3) + 2] = data[j + 0];
      texinfo->texels[(i * 3) + 1] = data[j + 1];
      texinfo->texels[(i * 3) + 0] = data[j + 2];
    }
}


static void
read_tga_32bits (GLubyte *data, struct gl_texture *texinfo)
{
  int i, j = 0;

  for (i = 0; i < texinfo->width * texinfo->height; ++i, j += 4)
    {
      /* Read and convert BGRA to RGBA */
      texinfo->texels[(i * 4) + 2] = data[j + 0];
      texinfo->texels[(i * 4) + 1] = data[j + 1];
      texinfo->texels[(i * 4) + 0] = data[j + 2];
      texinfo->texels[(i * 4) + 3] = data[j + 3];
    }
}


static void
read_tga_gray8bits (GLubyte *data, struct gl_texture *texinfo)
{
  memcpy (texinfo->texels, data, sizeof (GLubyte) *
	  texinfo->width * texinfo->height);
}


static void
read_tga_gray16bits (GLubyte *data, struct gl_texture *texinfo)
{
  memcpy (texinfo->texels, data, sizeof (GLubyte) *
	  texinfo->width * texinfo->height * 2);
}


static void
read_tga_8bits_rle (GLubyte *data, GLubyte *colormap, struct gl_texture *texinfo)
{
  int i, size, j = 0;
  GLubyte color;
  GLubyte packet_header;
  GLubyte *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height) * 3)
    {
      /* Read first byte */
      packet_header = data[j++];
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  color = data[j++];

	  for (i = 0; i < size; ++i, ptr += 3)
	    {
	      ptr[0] = colormap[(color * 3) + 2];
	      ptr[1] = colormap[(color * 3) + 1];
	      ptr[2] = colormap[(color * 3) + 0];
	    }
	}
      else
	{
	  /* Non run-length packet */
	  for (i = 0; i < size; ++i, ptr += 3)
	    {
	      color = data[j++];

	      ptr[0] = colormap[(color * 3) + 2];
	      ptr[1] = colormap[(color * 3) + 1];
	      ptr[2] = colormap[(color * 3) + 0];
	    }
	}
    }
}


static void
read_tga_16bits_rle (GLubyte *data, struct gl_texture *texinfo)
{
  int i, size, j = 0;
  unsigned short color;
  GLubyte packet_header;
  GLubyte *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height) * 3)
    {
      /* Read first byte */
      packet_header = data[j++];
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  color = data[j] + (data[j + 1] << 8);
	  j += 2;

	  for (i = 0; i < size; ++i, ptr += 3)
	    {
	      ptr[0] = (GLubyte)(((color & 0x7C00) >> 10) << 3);
	      ptr[1] = (GLubyte)(((color & 0x03E0) >>  5) << 3);
	      ptr[2] = (GLubyte)(((color & 0x001F) >>  0) << 3);
	    }
	}
      else
	{
	  /* Non run-length packet */
	  for (i = 0; i < size; ++i, ptr += 3, j += 2)
	    {
	      color = data[j] + (data[j + 1] << 8);

	      ptr[0] = (GLubyte)(((color & 0x7C00) >> 10) << 3);
	      ptr[1] = (GLubyte)(((color & 0x03E0) >>  5) << 3);
	      ptr[2] = (GLubyte)(((color & 0x001F) >>  0) << 3);
	    }
	}
    }
}


static void
read_tga_24bits_rle (GLubyte *data, struct gl_texture *texinfo)
{
  int i, size, j = 0;
  GLubyte *rgb;
  GLubyte packet_header;
  GLubyte *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height) * 3)
    {
      /* Read first byte */
      packet_header = data[j++];
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  rgb = &data[j];
	  j += 3;

	  for (i = 0; i < size; ++i, ptr += 3)
	    {
	      ptr[0] = rgb[2];
	      ptr[1] = rgb[1];
	      ptr[2] = rgb[0];
	    }
	}
      else
	{
	  /* Non run-length packet */
	  for (i = 0; i < size; ++i, ptr += 3, j += 3)
	    {
	      ptr[2] = data[j + 0];
	      ptr[1] = data[j + 1];
	      ptr[0] = data[j + 2];
	    }
	}
    }
}


static void
read_tga_32bits_rle (GLubyte *data, struct gl_texture *texinfo)
{
  int i, size, j = 0;
  GLubyte *rgba;
  GLubyte packet_header;
  GLubyte *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height) * 4)
    {
      /* Read first byte */
      packet_header = data[j++];
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  rgba = &data[j];
	  j += 4;

	  for (i = 0; i < size; ++i, ptr += 4)
	    {
	      ptr[0] = rgba[2];
	      ptr[1] = rgba[1];
	      ptr[2] = rgba[0];
	      ptr[3] = rgba[3];
	    }
	}
      else
	{
	  /* Non run-length packet */
	  for (i = 0; i < size; ++i, ptr += 4, j += 4)
	    {
	      ptr[2] = data[j + 0];
	      ptr[1] = data[j + 1];
	      ptr[0] = data[j + 2];
	      ptr[3] = data[j + 3];
	    }
	}
    }
}


static void
read_tga_gray8bits_rle (GLubyte *data, struct gl_texture *texinfo)
{
  int i, size, j = 0;
  GLubyte color;
  GLubyte packet_header;
  GLubyte *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height))
    {
      /* Read first byte */
      packet_header = data[j++];
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  color = data[j++];

	  for (i = 0; i < size; ++i, ptr++)
	    *ptr = color;
	}
      else
	{
	  /* Non run-length packet */
	  memcpy (ptr, data + j, size * sizeof (GLubyte));

	  ptr += size;
	  j += size;
	}
    }
}


static void
read_tga_gray16bits_rle (GLubyte *data, struct gl_texture *texinfo)
{
  int i, size, j = 0;
  GLubyte color, alpha;
  GLubyte packet_header;
  GLubyte *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height) * 2)
    {
      /* Read first byte */
      packet_header = data[j++];
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  color = data[j++];
	  alpha = data[j++];

	  for (i = 0; i < size; ++i, ptr += 2)
	    {
	      ptr[0] = color;
	      ptr[1] = alpha;
	    }
	}
      else
	{
	  /* Non run-length packet */
	  memcpy (ptr, data + j, size * sizeof (GLubyte) * 2);

	  ptr += size * 2;
	  j += size * 2;
	}
    }
}


static struct gl_texture *
read_tga_file (const char *filename)
{
  FILE *fp;
  struct gl_texture *texinfo;
  struct tga_header *header;
  GLubyte *buffer = NULL;
  GLubyte *colormap = NULL;
  GLubyte *ptr;
  long length;

  /* Open file */
  fp = fopen (filename, "rb");
  if (!fp)
    {
      fprintf (stderr, "Error: couldn't open \"%s\"!\n %s\n",
	       filename, strerror (errno));
      return NULL;
    }

  /* Get file length */
  fseek (fp, 0, SEEK_END);
  length = ftell (fp);
  fseek (fp, 0, SEEK_SET);

  buffer = (GLubyte *) malloc (sizeof (GLubyte) * length);
  if (!buffer)
    {
      fprintf (stderr, "Error: memory allocation failed "
	       "for \"%s\"\n %s\n", filename, strerror (errno));
      fclose (fp);
      return NULL;
    }

  /* Read whole shader code */
  fread (buffer, sizeof (GLubyte), length, fp);
  fclose (fp);

  ptr = buffer;

  /* Extract header */
  header = (struct tga_header *)ptr;
  ptr += sizeof (struct tga_header);

  texinfo = (struct gl_texture *)malloc (sizeof (struct gl_texture));
  get_texture_info (header, texinfo);
  ptr += header->id_lenght;

  /* Memory allocation for pixel data */
  texinfo->texels = (GLubyte *)malloc (sizeof (GLubyte) *
	texinfo->width * texinfo->height * texinfo->internalFormat);
  if (!texinfo->texels)
    {
      free (texinfo);
      return NULL;
    }

  /* Read color map */
  if (header->colormap_type)
    {
      /* NOTE: color map is stored in BGR format */
      colormap = ptr;
      ptr += header->cm_length * (header->cm_size >> 3);
    }

  /* Read image data */
  switch (header->image_type)
    {
    case 0:
      /* No data */
      break;

    case 1:
      /* Uncompressed 8 bits color index */
      read_tga_8bits (ptr, colormap, texinfo);
      break;

    case 2:
      /* Uncompressed 16-24-32 bits */
      switch (header->pixel_depth)
	{
	case 16:
	  read_tga_16bits (ptr, texinfo);
	  break;

	case 24:
	  read_tga_24bits (ptr, texinfo);
	  break;

	case 32:
	  read_tga_32bits (ptr, texinfo);
	  break;
	}

      break;

    case 3:
      /* Uncompressed 8 or 16 bits grayscale */
      if (header->pixel_depth == 8)
	read_tga_gray8bits (ptr, texinfo);
      else /* 16 */
	read_tga_gray16bits (ptr, texinfo);

      break;

    case 9:
      /* RLE compressed 8 bits color index */
      read_tga_8bits_rle (ptr, colormap, texinfo);
      break;

    case 10:
      /* RLE compressed 16-24-32 bits */
      switch (header->pixel_depth)
	{
	case 16:
	  read_tga_16bits_rle (ptr, texinfo);
	  break;

	case 24:
	  read_tga_24bits_rle (ptr, texinfo);
	  break;

	case 32:
	  read_tga_32bits_rle (ptr, texinfo);
	  break;
	}

      break;

    case 11:
      /* RLE compressed 8 or 16 bits grayscale */
      if (header->pixel_depth == 8)
	read_tga_gray8bits_rle (ptr, texinfo);
      else /* 16 */
	read_tga_gray16bits_rle (ptr, texinfo);

      break;

    default:
      /* Image type is not correct */
      fprintf (stderr, "Error: unknown TGA image type %i!\n",
	       header->image_type);
      free (texinfo->texels);
      free (texinfo);
      texinfo = NULL;
      break;
    }

  if (buffer)
    free (buffer);

  return texinfo;
}


int
load_tga_texture (const char *filename, GLenum target)
{
  struct gl_texture *tga_tex = NULL;

  /* Read image from file */
  tga_tex = read_tga_file (filename);

  if (tga_tex && tga_tex->texels)
    {
      /* Build texture from image data */
      glTexImage2D (target, 0, tga_tex->internalFormat,
		    tga_tex->width, tga_tex->height, 0, tga_tex->format,
		    GL_UNSIGNED_BYTE, tga_tex->texels);

      /* OpenGL has its own copy of texture data */
      free (tga_tex->texels);
      free (tga_tex);

      return 1;
    }

  return 0;
}
