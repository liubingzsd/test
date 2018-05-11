#include <stdio.h>
#include "mat_math.h"
#include "svd_solver.h"
void ppp(double *a, double *e, double *s, double *v, int m, int n);
void sss(double *fg, double *cs);
bool svd_cmp(double *a, int m, int n, double *u, double *v, double eps, int ka)
{
	int i, j, k, l, it, ll, kk, ix, iy, mm, nn, iz, m1, ks;
	double d, dd, t, sm, sm1, em1, sk, ek, b, c, shh, fg[2], cs[2];
	double *s, *e, *w;
	void ppp();
	void sss();

	s = malloc(ka * sizeof(double));
	e = malloc(ka * sizeof(double));
	w = malloc(ka * sizeof(double));

	it = 60;
	k = n;
	if ((m - 1) < n)
	{
		k = m - 1;                // k = min(n,m-1)
	}
	l = m;
	if ((n - 2) < m)
	{
		l = n - 2;               // l = min(m,n-2)
	}
	if (l < 0)
	{
		l = 0;
	}
	ll = k;
	if (l > k)
	{
		ll = l;
	}
	if (ll >= 1)
	{
		for (kk = 1; kk <= ll; kk++)
		{
			if (kk <= k)
			{
				d = 0.0;
				for (i = kk; i <= m; i++)
				{
					ix = (i - 1) * n + kk - 1;
					d = d + a[ix] * a[ix];
				}
				s[kk - 1] = sqrt(d);
				if (s[kk - 1] != 0.0)
				{
					ix = (kk - 1)*n + kk - 1;
					if (a[ix] != 0.0)
					{
						s[kk - 1] = fabs(s[kk - 1]);
						if (a[ix] < 0.0)
						{
							s[kk - 1] = -s[kk - 1];
						}
					}
					for (i = kk; i <= m; i++)
					{
						iy = (i - 1)*n + kk - 1;
						a[iy] = a[iy] / s[kk - 1];
					}
					a[ix] = 1.0 + a[ix];
				}
				s[kk - 1] = -s[kk - 1];
			}
			if (n >= kk + 1)
			{
				for (j = kk + 1; j <= n; j++)
				{
					if ((kk <= k) && (s[kk - 1] != 0.0))
					{
						d = 0.0;
						for (i = kk; i <= m; i++)
						{
							ix = (i - 1)*n + kk - 1;
							iy = (i - 1)*n + j - 1;
							d = d + a[ix] * a[iy];
						}
						d = -d / a[(kk - 1)*n + kk - 1];
						for (i = kk; i <= m; i++)
						{
							ix = (i - 1)*n + j - 1;
							iy = (i - 1)*n + kk - 1;
							a[ix] = a[ix] + d * a[iy];
						}
					}
					e[j - 1] = a[(kk - 1)*n + j - 1];
				}
			}
			if (kk <= k)
			{
				for (i = kk; i <= m; i++)
				{
					ix = (i - 1)*m + kk - 1;
					iy = (i - 1)*n + kk - 1;
					u[ix] = a[iy];
				}
			}
			if (kk <= l)
			{
				d = 0.0;
				for (i = kk + 1; i <= n; i++)
				{
					d = d + e[i - 1] * e[i - 1];
				}
				e[kk - 1] = sqrt(d);
				if (e[kk - 1] != 0.0)
				{
					if (e[kk] != 0.0)
					{
						e[kk - 1] = fabs(e[kk - 1]);
						if (e[kk] < 0.0)
						{
							e[kk - 1] = -e[kk - 1];
						}
					}
					for (i = kk + 1; i <= n; i++)
					{
						e[i - 1] = e[i - 1] / e[kk - 1];
					}					
					e[kk] = 1.0 + e[kk];
				}
				e[kk - 1] = -e[kk - 1];
				if ((kk + 1 <= m) && (e[kk - 1] != 0.0))
				{
					for (i = kk + 1; i <= m; i++)
					{
						w[i - 1] = 0.0;
					}
					for (j = kk + 1; j <= n; j++)
					{
						for (i = kk + 1; i <= m; i++)
						{
							w[i - 1] = w[i - 1] + e[j - 1] * a[(i - 1)*n + j - 1];
						}
							
					}
					for (j = kk + 1; j <= n; j++)
					{
						for (i = kk + 1; i <= m; i++)
						{
							ix = (i - 1)*n + j - 1;
							a[ix] = a[ix] - w[i - 1] * e[j - 1] / e[kk];
						}
					}

				}
				for (i = kk + 1; i <= n; i++)
				{
					v[(i - 1)*n + kk - 1] = e[i - 1];
				}				
			}
		}
	}

	mm = n;
	if ((m + 1) < n)
	{
		mm = m + 1;
	}
	if (k < n)
	{
		s[k] = a[k*n + k];
	}
	if (m < mm)
	{
		s[mm - 1] = 0.0;
	}
	if ((l + 1) < mm)
	{
		e[l] = a[l*n + mm - 1];
	}
	e[mm - 1] = 0.0;
	nn = m;
	if (m > n)
	{
		nn = n;
	}
	if (nn >= k + 1)
	{
		for (j = k + 1; j <= nn; j++)
		{
			for (i = 1; i <= m; i++)
			{
				u[(i - 1)*m + j - 1] = 0.0;
			}
			u[(j - 1)*m + j - 1] = 1.0;
		}
	}
	if (k >= 1)
	{
		for (ll = 1; ll <= k; ll++)
		{
			kk = k - ll + 1; 
			iz = (kk - 1)*m + kk - 1;
			if (s[kk - 1] != 0.0)
			{
				if (nn >= kk + 1)
					for (j = kk + 1; j <= nn; j++)
					{
						d = 0.0;
						for (i = kk; i <= m; i++)
						{
							ix = (i - 1)*m + kk - 1;
							iy = (i - 1)*m + j - 1;
							d = d + u[ix] * u[iy] / u[iz];
						}
						d = -d;
						for (i = kk; i <= m; i++)
						{
							ix = (i - 1)*m + j - 1;
							iy = (i - 1)*m + kk - 1;
							u[ix] = u[ix] + d * u[iy];
						}
					}
				for (i = kk; i <= m; i++)
				{
					ix = (i - 1)*m + kk - 1;
					u[ix] = -u[ix];
				}
				u[iz] = 1.0 + u[iz];
				if (kk - 1 >= 1)
				{
					for (i = 1; i <= kk - 1; i++)
					{
						u[(i - 1)*m + kk - 1] = 0.0;
					}
				}
			}
			else
			{
				for (i = 1; i <= m; i++)
				{
					u[(i - 1)*m + kk - 1] = 0.0;
				}
				u[(kk - 1)*m + kk - 1] = 1.0;
			}
		}
	}

	for (ll = 1; ll <= n; ll++)
	{
		kk = n - ll + 1; iz = kk * n + kk - 1;
		if ((kk <= l) && (e[kk - 1] != 0.0))
		{
			for (j = kk + 1; j <= n; j++)
			{
				d = 0.0;
				for (i = kk + 1; i <= n; i++)
				{
					ix = (i - 1)*n + kk - 1; 
					iy = (i - 1)*n + j - 1;
					d = d + v[ix] * v[iy] / v[iz];
				}
				d = -d;
				for (i = kk + 1; i <= n; i++)
				{
					ix = (i - 1)*n + j - 1; 
					iy = (i - 1)*n + kk - 1;
					v[ix] = v[ix] + d * v[iy];
				}
			}
		}
		for (i = 1; i <= n; i++)
		{
			v[(i - 1)*n + kk - 1] = 0.0;
		}
		v[iz - n] = 1.0;
	}
	for (i = 1; i <= m; i++)
	{
		for (j = 1; j <= n; j++)
		{
			a[(i - 1)*n + j - 1] = 0.0;
		}
	}
	m1 = mm;
	it = 60;
	while (1)
	{
		if (mm == 0)
		{
			ppp(a, e, s, v, m, n);
			free(s);
			free(e);
			free(w); 
			return true;
		}
		if (it == 0)
		{
			ppp(a, e, s, v, m, n);
			free(s);
			free(e);
			free(w);
			return false;
		}
		kk = mm - 1;
		while ((kk != 0) && (fabs(e[kk - 1]) != 0.0))
		{
			d = fabs(s[kk - 1]) + fabs(s[kk]);
			dd = fabs(e[kk - 1]);
			if (dd > eps*d)
			{
				kk = kk - 1;
			}
			else
			{
				e[kk - 1] = 0.0;
			}
		}
		if (kk == mm - 1)
		{
			kk = kk + 1;
			if (s[kk - 1]<0.0)
			{
				s[kk - 1] = -s[kk - 1];
				for (i = 1; i <= n; i++)
				{
					ix = (i - 1)*n + kk - 1;
					v[ix] = -v[ix];
				}
			}
			while ((kk != m1) && (s[kk - 1]<s[kk]))
			{
				d = s[kk - 1]; 
				s[kk - 1] = s[kk]; 
				s[kk] = d;
				if (kk < n)
					for (i = 1; i <= n; i++)
					{
						ix = (i - 1)*n + kk - 1;
						iy = (i - 1)*n + kk;
						d = v[ix];
						v[ix] = v[iy]; 
						v[iy] = d;
					}
				if (kk < m)
					for (i = 1; i <= m; i++)
					{
						ix = (i - 1)*m + kk - 1;
						iy = (i - 1)*m + kk;
						d = u[ix];
						u[ix] = u[iy];
						u[iy] = d;
					}
				kk = kk + 1;
			}
			it = 60;
			mm = mm - 1;
		}
		else
		{
			ks = mm;
			while ((ks>kk) && (fabs(s[ks - 1]) != 0.0))
			{
				d = 0.0;
				if (ks != mm)
				{
					d = d + fabs(e[ks - 1]);
				}
				if (ks != kk + 1)
				{
					d = d + fabs(e[ks - 2]);
				}
				dd = fabs(s[ks - 1]);
				if (dd > eps*d)
				{
					ks = ks - 1;
				}
				else
				{
					s[ks - 1] = 0.0;
				}
			}
			if (ks == kk)
			{
				kk = kk + 1;
				d = fabs(s[mm - 1]);
				t = fabs(s[mm - 2]);
				if (t > d)
				{
					d = t;
				}
				t = fabs(e[mm - 2]);
				if (t > d)
				{
					d = t;
				}
				t = fabs(s[kk - 1]);
				if (t > d)
				{
					d = t;
				}
				t = fabs(e[kk - 1]);
				if (t > d)
				{
					d = t;
				}
				sm = s[mm - 1] / d; 
				sm1 = s[mm - 2] / d;
				em1 = e[mm - 2] / d;
				sk = s[kk - 1] / d;
				ek = e[kk - 1] / d;
				b = ((sm1 + sm)*(sm1 - sm) + em1 * em1) / 2.0;
				c = sm * em1; 
				c = c * c; 
				shh = 0.0;
				if ((b != 0.0) || (c != 0.0))
				{
					shh = sqrt(b*b + c);
					if (b < 0.0)
					{
						shh = -shh;
					}
					shh = c / (b + shh);
				}
				fg[0] = (sk + sm)*(sk - sm) - shh;
				fg[1] = sk * ek;
				for (i = kk; i <= mm - 1; i++)
				{
					sss(fg, cs);
					if (i != kk)
					{
						e[i - 2] = fg[0];
					}
					fg[0] = cs[0] * s[i - 1] + cs[1] * e[i - 1];
					e[i - 1] = cs[0] * e[i - 1] - cs[1] * s[i - 1];
					fg[1] = cs[1] * s[i];
					s[i] = cs[0] * s[i];
					if ((cs[0] != 1.0) || (cs[1] != 0.0))
						for (j = 1; j <= n; j++)
						{
							ix = (j - 1)*n + i - 1;
							iy = (j - 1)*n + i;
							d = cs[0] * v[ix] + cs[1] * v[iy];
							v[iy] = -cs[1] * v[ix] + cs[0] * v[iy];
							v[ix] = d;
						}
					sss(fg, cs);
					s[i - 1] = fg[0];
					fg[0] = cs[0] * e[i - 1] + cs[1] * s[i];
					s[i] = -cs[1] * e[i - 1] + cs[0] * s[i];
					fg[1] = cs[1] * e[i];
					e[i] = cs[0] * e[i];
					if (i<m)
						if ((cs[0] != 1.0) || (cs[1] != 0.0))
						{
							for (j = 1; j <= m; j++)
							{
								ix = (j - 1)*m + i - 1;
								iy = (j - 1)*m + i;
								d = cs[0] * u[ix] + cs[1] * u[iy];
								u[iy] = -cs[1] * u[ix] + cs[0] * u[iy];
								u[ix] = d;
							}
						}

				}
				e[mm - 2] = fg[0];
				it = it - 1;
			}
			else
			{
				if (ks == mm)
				{
					kk = kk + 1;
					fg[1] = e[mm - 2]; e[mm - 2] = 0.0;
					for (ll = kk; ll <= mm - 1; ll++)
					{
						i = mm + kk - ll - 1;
						fg[0] = s[i - 1];
						sss(fg, cs);
						s[i - 1] = fg[0];
						if (i != kk)
						{
							fg[1] = -cs[1] * e[i - 2];
							e[i - 2] = cs[0] * e[i - 2];
						}
						if ((cs[0] != 1.0) || (cs[1] != 0.0))
							for (j = 1; j <= n; j++)
							{
								ix = (j - 1)*n + i - 1;
								iy = (j - 1)*n + mm - 1;
								d = cs[0] * v[ix] + cs[1] * v[iy];
								v[iy] = -cs[1] * v[ix] + cs[0] * v[iy];
								v[ix] = d;
							}
					}
				}
				else
				{
					kk = ks + 1;
					fg[1] = e[kk - 2];
					e[kk - 2] = 0.0;
					for (i = kk; i <= mm; i++)
					{
						fg[0] = s[i - 1];
						sss(fg, cs);
						s[i - 1] = fg[0];
						fg[1] = -cs[1] * e[i - 1];
						e[i - 1] = cs[0] * e[i - 1];
						if ((cs[0] != 1.0) || (cs[1] != 0.0))
						{
							for (j = 1; j <= m; j++)
							{
								ix = (j - 1)*m + i - 1;
								iy = (j - 1)*m + kk - 2;
								d = cs[0] * u[ix] + cs[1] * u[iy];
								u[iy] = -cs[1] * u[ix] + cs[0] * u[iy];
								u[ix] = d;
							}
						}

					}
				}
			}
		}
	}
	printf("svd cmp finished \n");
	return true;
}

void ppp(double *a, double *e, double *s, double *v, int m, int n)
{
	int i, j, p, q;
	double d;
	if (m >= n)
	{
		i = n;
	}
	else
	{
		i = m;
	}
	for (j = 1; j <= i - 1; j++)
	{
		a[(j - 1)*n + j - 1] = s[j - 1];
		a[(j - 1)*n + j] = e[j - 1];
	}
	a[(i - 1)*n + i - 1] = s[i - 1];
	if (m < n)
	{
		a[(i - 1)*n + i] = e[i - 1];
	}
	for (i = 1; i <= n - 1; i++)
	{
		for (j = i + 1; j <= n; j++)
		{
			p = (i - 1)*n + j - 1; 
			q = (j - 1)*n + i - 1;
			d = v[p]; v[p] = v[q]; v[q] = d;
		}
	}
}

void sss(double *fg, double *cs)
{ 
	double r, d;
	if ((fabs(fg[0]) + fabs(fg[1])) == 0.0)
	{
		cs[0] = 1.0;
		cs[1] = 0.0;
		d = 0.0;
	}
	else
	{
		d = sqrt(fg[0] * fg[0] + fg[1] * fg[1]);
		if (fabs(fg[0])>fabs(fg[1]))
		{
			d = fabs(d);
			if (fg[0] < 0.0)
			{
				d = -d;
			}
		}
		if (fabs(fg[1]) >= fabs(fg[0]))
		{
			d = fabs(d);
			if (fg[1] < 0.0)
			{
				d = -d;
			}
		}
		cs[0] = fg[0] / d;
		cs[1] = fg[1] / d;
	}
	r = 1.0;
	if (fabs(fg[0]) > fabs(fg[1])) 
	{
		r = cs[1];
	}
	else 
	{
		if (cs[0] != 0.0)
		{
			r = 1.0 / cs[0];
		}
	}		
	fg[0] = d;
	fg[1] = r;

}
double norm_vector(double *f,int length)
{
	int i;
	double s = 0.0f;
	for (i = 0; i < length; i++)
	{
		s += f[i] * f[i];
	}
	s = sqrt(s);
	return s;
}
void householder(double *a,int length,double *p)
{
	double e[10] = { 0.0f };
	double norm_vec;
	double w_w[10 * 10] = {0};
	int i,j;
	vector_create(e, length);
	e[0] = 1.0f;
	norm_vec = norm_vector(a, length);
	if (a[0] < 0)
	{
		norm_vec = -norm_vec;
	}
	sv_mul(e, length, -norm_vec);
	vv_add(a,e,a,length);
	norm_vec = norm_vector(a, length);
	sv_mul(a,length,1/norm_vec);
	vv_mul_mat(a, a, w_w,length);
	for (i = 0; i < length; i++)
	{
		for (j = 0; j < length;j++)
		{
			if (i == j)
			{
				p[i * length + j] = 1.0f - 2.0f * w_w[i*length + j];
			}
			else
			{
				p[i * length + j] = -2.0f * w_w[i*length + j];
			}
		}
	}
}

bool svd_ginv(double *a,int m,int n,double *ginv,double eps,double *u,double *v,int ka)
{
	int i, j, k, l, t, p, q, f;
	int tmp_length;
	if (svd_cmp(a, m, n, u, v, eps, ka))
	{
		printf("svd_ginv \n");
		printf("U is  \n");
		mat_printf(u, m, m);
		printf("S is  \n");
		mat_printf(a, m, n);
		printf("V is  \n");
		mat_printf(v, n, n);
		printf("USV end \n");

		j = n;
		if (m < n)
		{
			j = m;
		}
		j = j - 1;
		k = 0;
		if (m >= n)
		{
			tmp_length = n;
		}
		else
		{
			tmp_length = m;
		}
		for (i = 0; i < n; i++)
		{
			if (fabs(a[i * n + i]) < 0.0000001)
			{
				a[i * n + i] = 0;
			}
		}
//		printf("norm S is  \n");
//		mat_printf(a, m, n);
		printf("\n");
		while ((k <= j) && (a[k*n + k] != 0.0))
		{
			k = k + 1;
		}
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < m; j++)
			{
				t = i * m + j; 
				ginv[t] = 0.0f;
				for (l = 0; l < k; l++)
				{
					f = l * n + i;
					p = j * m + l;
					q = l * n + l;
					ginv[t] += v[f] * u[p] / a[q];
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}

}



/*
 a*x = b
 a = u*sigma*v;
 u[m][n]
 v[n][n];
*/
bool svd_solver(double *a,int m,int n,double *b,double *x,double *ginv,double eps,double *u,double *v,int ka)
{
	int i, j;
	if (svd_ginv(a, m, n, ginv, eps, u, v, ka))
	{
		for (i = 0; i < n; i++)
		{
			x[i] = 0.0;
			for (j = 0; j <= m - 1; j++)
			{
				x[i] += ginv[i*m + j] * b[j];
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}


void test_svd_cmp()
{
	double a[4][3] = {
		{ 1.0,1.0,-1.0 },
		{ 2.0,1.0,0.0 },
		{ 1.0,-1.0,0.0 },
		{ -1.0,2.0,1.0 }
	};
	double b[3][4] = {
		{ 1.0,1.0,-1.0,-1.0 },
		{ 2.0,1.0,0.0,2.0 },
		{ 1.0,-1.0,0.0,1.0 }
	};
	double u[4][4] = { 0.0f }, v[3][3], c[4][3], d[3][4];
	double eps = 0.000001;
	if (svd_cmp((double *)a, 4, 3, (double *)u, (double *)v, eps, 5))
	{

		printf("\n");
		printf("EXAMPLE(1)\n");
		printf("\n");
		printf("MAT U IS:\n");
		mat_printf(&u[0][0], 4, 4);
		printf("\n");
		printf("MAT V IS:\n");
		mat_printf(&v[0][0], 3, 3);
		printf("\n");
		printf("MAT A IS:\n");
		mat_printf(&a[0][0], 4, 3);
		printf("\n\n");
		printf("MAT UAV IS:\n");
		mm_mul(&u[0][0], &a[0][0], &c[0][0], 4, 4, 3);
		mm_mul(&c[0][0], &v[0][0], &a[0][0], 4, 3, 3);
		mat_printf(&a[0][0], 4, 3);
		printf("\n\n");
		printf("EXAMPLE(2)\n");
		printf("\n");
	}

	if (svd_cmp(&b[0][0], 3, 4, &v[0][0], &u[0][0], eps, 5))
	{
		printf("MAT U IS:\n");
		mat_printf(&v[0][0], 3, 3);
		printf("\n");
		printf("MAT V IS:\n");
		mat_printf(&u[0][0], 4, 4);
		printf("\n");
		printf("MAT B IS:\n");
		mat_printf(&b[0][0], 3, 4);
		printf("\n\n");
		printf("MAT UBV IS:\n");
		mm_mul(&v[0][0], &b[0][0], &d[0][0], 3, 3, 4);
		mm_mul(&d[0][0], &u[0][0], &b[0][0], 3, 4, 4);
		mat_printf(&b[0][0], 3, 4);
		printf("\n");
	}
}

void test_svd_ginv()
{
	int m, n, ka;
#if 1
	double a[8][5] = {
	{ 22,10,2,3,7 },
	{ 14,7,10,0,8 },
	{ -1,13,-1,-11,3 },
	{ -3,-2,13,-2,4 },
	{ 9,8,1,-2,4 },
	{ 9,1,-7,5,-1 },
	{ 2,-6,6,5,1 },
	{ 4,5,0,-2,2 }
	};

	double ginv[5][8] = { 0 };

#endif
#if 0
	double ginv[5][8] = {
	{0.0211,0.0093,-0.0111,-0.0079,0.0055,0.0143,0.0049,0.0015 },
	{0.0046,0.0022,0.0274,-0.0050,0.0098,-0.0026,-0.0150,0.0074},
	{-0.0021,0.0205,-0.0039,0.0338,-0.0009,-0.0201,0.0153,-0.0017},
	{0.0076, -0.0002,-0.0276,-0.0054,-0.0050,0.0128,0.0124,-0.0050},
	{0.0038,0.0100,0.0042,0.0104,0.0032,-0.0062,0.0026,0.0016}
	};
	double a[8][5] = { 0 };
#endif

	double c[8][5] = { 0 }, u[8][8] = { 0 }, v[5][5] = { 0 };
	double eps = 0.000001;
	m = 8; n = 5; ka = 9; 
	printf("test svd_ginv \n");
	printf("MAT A IS:\n");
	mat_printf((double *)a, m, n);
	printf("\n");

	printf("MAT A+ IS:\n");
	if (svd_ginv((double *)a, m, n, (double *)ginv, eps, (double *)u, (double *)v, ka))
	{
		mat_printf((double *)ginv, n, m);
	}

	printf("MAT A++ IS:\n");
	if (svd_ginv((double *)ginv, n, m, (double *)c, eps, (double *)v, (double *)u, ka))
	{
		printf("calc ginv succeed \n ");
		mat_printf((double *)c, m, n);
	}
	else
	{
		printf("calc ginv failed \n ");
	}
	printf("test svd ginv end \n");
}


void test_svd_solver_1()
{
	int i, m, n, ka;
	double x[3], ginv[3][4], u[4][4], v[3][3];
	double a[4][3] = { 
		{ 1.0,1.0,-1.0 },
		{ 2.0,1.0,0.0 },
		{ 1.0,-1.0,0.0 },
		{ -1.0,2.0,1.0 } 
	};
	double b[4] = {2.0,-3.0,1.0,4.0 };
	double eps = 0.000001;
	m = 4; n = 3; ka = 5;
	if (svd_solver((double *)a, m, n, b, x, (double *)ginv, eps, (double *)u, (double *)v, ka));
	{
		for (i = 0; i < 3; i++)
		{
			printf("x(%d) = %12.7f\n", i, x[i]);
		}
		printf("\n");
		printf("The ginv of mat a is  :\n");
		mat_printf((double *)ginv, n, m);
		printf("\n");
	}
}



void test_svd_solver_2()
{
	int i, m, n, ka;
	double x[4], ginv[4][4], u[4][4], v[4][4];
	double a[4][4] = {
	{ 0.2368,0.2471,0.2568,1.2671 },
	{ 0.1968,0.2071,1.2168,0.2271 },
	{ 0.1581,1.1675,0.1768,0.1871 },
	{ 1.1161,0.1254,0.1397,0.1490 } };
	double b[4] = { 1.8471,1.7471,1.6471,1.5471 };
	double eps = 0.000001;
	m = 4; n = 4; ka = 5;
	if (svd_solver((double *)a, m, n, b, x, (double *)ginv, eps, (double *)u, (double *)v, ka));
	{
		for (i = 0; i < n; i++)
		{
			printf("x(%d) = %12.7f\n", i, x[i]);
		}
		printf("\n");
		printf("The ginv of mat a is  :\n");
		mat_printf((double *)ginv, m, m);
		printf("\n");
	}
}



void test_svd_solver_3()
{
	#define M 8
	#define N 5
	int i, ka;
	double x[N], ginv[N][M], u[M][M], v[N][N];
	double a[M][N] = {
		{22,10,2,3,7},
		{14,7,10,0,8},
		{-1,13,-1,-11,3},
		{-3,-2,13,-2,4},
		{9,8,1,-2,4},
		{9,1,-7,5,-1},
		{2,-6,6,5,1},
		{4,5,0,-2,2}
	};
	double b[M] = {-1,2,1,4,0,-3,1,0};
//	double b[N] = { 1,-1,10,0,-6,6,11,-5 };
//	double b[N] = { 0,1,11,4,-6,3,12,-5 };
	double eps = 0.000001;
	ka = 9;
	if (svd_solver((double *)a, M, N, b, x, (double *)ginv, eps, (double *)u, (double *)v, ka));
	{
		for (i = 0; i < N; i++)
		{
			printf("x(%d) = %12.7f\n", i, x[i]);
		}
		printf("\n");
		printf("The ginv of mat a is  :\n");
		mat_printf((double *)ginv, N, M);
		printf("\n");
	}
}


void test_svd_cmp_1()
{
	printf("test svd cmp \n");
	double a[8][5] = {
		{ 22,10,2,3,7 },
		{ 14,7,10,0,8 },
		{ -1,13,-1,-11,3 },
		{ -3,-2,13,-2,4 },
		{ 9,8,1,-2,4 },
		{ 9,1,-7,5,-1 },
		{ 2,-6,6,5,1 },
		{ 4,5,0,-2,2 }
	};
	double u[8][8] = { 0.0f }, v[5][5], c[8][5];
	double eps = 0.000001;
	printf("test svd cmp s1\n");
	int ka = 9;
	if (svd_cmp((double *)a, 8, 5, (double *)u, (double *)v, eps, 9))
	{

		printf("\n");
		printf("EXAMPLE(1)\n");
		printf("\n");
		printf("MAT U IS:\n");
		mat_printf(&u[0][0], 8, 8);
		printf("\n");
		printf("MAT V IS:\n");
		mat_printf(&v[0][0], 5,5);
		printf("\n");
		printf("MAT A IS:\n");
		mat_printf(&a[0][0], 8, 5);
		printf("\n\n");
		printf("MAT UAV IS:\n");
		mm_mul(&u[0][0], &a[0][0], &c[0][0], 8, 8, 5);
		mm_mul(&c[0][0], &v[0][0], &a[0][0], 8, 5, 5);
		mat_printf(&a[0][0], 8, 5);
		printf("\n\n");
	}
}

