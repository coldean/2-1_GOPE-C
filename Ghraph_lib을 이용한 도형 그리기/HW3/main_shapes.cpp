#include "Graph.h"
#include "Simple_window.h"

int main()
{
	using namespace Graph_lib;

	Point t1(100, 200);

	Simple_window win(t1, 800, 800, "Box");

	Vector_ref<Shape> b;
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
		{
			b.push_back(new Box(Point(50 + 170 * (j - 1), 50 + 170 * (i - 1)), 50 * j, 50 * i, 10));
			b[b.size() - 1].set_fill_color((i - 1) * 4 + j - 1);
			win.attach(b[b.size() - 1]);
		}
	win.wait_for_button();
	for (int i = 0; i < b.size(); i++)
		win.detach(b[i]);

	win.set_label("Regualr polygon");
	Vector_ref<Shape> p;
	int k = 3;
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
		{
			p.push_back(new Regular_polygon(Point(100 + 200 * (j - 1), 170 * i -30), k, 80));
			k++;
			p[p.size() - 1].set_color((i - 1) * 4 + j - 1);
			win.attach(p[p.size() - 1]);
		}
	win.wait_for_button();
	for (int i = 0; i < p.size(); i++)
		win.detach(p[i]);

	win.set_label("Star");
	Vector_ref<Shape> s;
	k = 3;
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
		{
			s.push_back(new Star(Point(100 + 200 * (j - 1), 170 * i - 30), k, 40,80));
			k++;
			s[s.size() - 1].set_color((i - 1) * 4 + j - 1);
			win.attach(s[s.size() - 1]);
		}
	win.wait_for_button();
	for (int i = 0; i < s.size(); i++)
		win.detach(s[i]);

	win.set_label("Rose");
	Vector_ref<Shape> r;
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
		{
			r.push_back(new Rose(Point(100 + 200 * (j - 1), 170 * i - 30), i, j, 80));
			r[r.size() - 1].set_color((i - 1) * 4 + j - 1);
			win.attach(r[r.size() - 1]);
		}
	win.wait_for_button();
	for (int i = 0; i < r.size(); i++)
		win.detach(r[i]);

	win.set_label("Cycloid");
	Vector_ref<Shape> c;
	k = 3;
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
		{
			c.push_back(new Cycloid(Point(20 + 200 * (j - 1), 170 * i - 30), 10, 50 * k));
			k++;
			c[c.size() - 1].set_color((i - 1) * 4 + j - 1);
			win.attach(c[c.size() - 1]);
		}
	win.wait_for_button();

}