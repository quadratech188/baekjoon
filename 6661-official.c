#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXS 10000

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct crane
{
  unsigned last_seg;		/* Posledni segment.  */
  double len, angle;		/* Delka a uhlel ke konci useku.  */
  double angle_delta;		/* Nezpropagovane pootoceni.  */
  struct crane *l, *r;		/* Podjeraby.  */
};

/* Zpropaguje angle_delta do synu.  */

void cleanup (struct crane *c)
{
  c->l->angle += c->angle_delta;
  c->l->angle_delta += c->angle_delta;
  c->r->angle += c->angle_delta;
  c->r->angle_delta += c->angle_delta;
  c->angle_delta = 0;
}

/* Spocita angle a len z hodnot pro syny.  */

void recompute (struct crane *c)
{
  double x, y;

  x = c->l->len * cos (c->l->angle);
  y = c->l->len * sin (c->l->angle);
  x += c->r->len * cos (c->r->angle);
  y += c->r->len * sin (c->r->angle);

  c->len = sqrt (x * x + y * y);
  c->angle = atan2 (y, x);
}

/* Zrotuje segment SEG o uhel DIR.  */

void rotate_segment (struct crane *c, unsigned seg, double dir)
{
  if (!c->l)
    {
      if (c->last_seg != seg)
	abort ();

      c->angle += dir;
      return;
    }

  cleanup (c);

  if (c->l->last_seg < seg)
    {
      rotate_segment (c->r, seg, dir);
      recompute (c);
      return;
    }

  rotate_segment (c->l, seg, dir);
  c->r->angle += dir;
  c->r->angle_delta += dir;
  recompute (c);
}

/* Urci uhel segmentu SEG.  */

double get_angle (struct crane *c, unsigned seg)
{
  if (!c->l)
    {
      if (c->last_seg != seg)
	abort ();
      
      return c->angle;
    }

  cleanup (c);
  return c->l->last_seg < seg ? get_angle (c->r, seg) : get_angle (c->l, seg);
}

double lengths[MAXS];

/* Sestavi strom segmentu z prvku od FROM do TO.  */
struct crane *build_crane (unsigned from, unsigned to)
{
  struct crane *n = malloc (sizeof (struct crane));
  unsigned mid = (from + to) / 2;

  n->last_seg = to;
  n->angle_delta = 0;
  n->angle = 0;
  
  if (from == to)
    {
      n->len = lengths[to];
      n->l = n->r = NULL;
      return n;
    }

  n->l = build_crane (from, mid);
  n->r = build_crane (mid + 1, to);
  n->len = n->l->len + n->r->len;

  return n;
}

void free_crane (struct crane *cr)
{
  if (cr->l)
    {
      free_crane (cr->l);
      free_crane (cr->r);
    }

  free (cr);
}

int main(void)
{
  unsigned i, n, c, l, s, a;
  double aangle, angle;
  struct crane *cr;
  int first = 1;

  while (1)
    {
      if (scanf ("%u%u", &n, &c) != 2)
	return 0;

	    if (!first)
		    printf ("\n");
	    first = 0;
      for (i = 0; i < n; i++)
	{
	  scanf ("%u", &l);
	  lengths[i] = l;
	}

      cr = build_crane (0, n - 1);
      rotate_segment (cr, 0, M_PI/2);
      for (i = 0; i < c; i++)
	{
	  scanf ("%u%u", &s, &a);
	  s--;
	  aangle = get_angle (cr, s + 1) - get_angle (cr, s) + M_PI;
	  angle = M_PI * a / 180;

	  rotate_segment (cr, s + 1, angle - aangle);

	  printf ("%.2f %.2f\n", cr->len * cos (cr->angle), cr->len * sin (cr->angle));
	}
      free_crane (cr);
    }
}

