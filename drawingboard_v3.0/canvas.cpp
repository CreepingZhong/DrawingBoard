﻿#include "canvas.h"
#include "CG_Painter.h"
#include<QDebug>
#include<fstream>
#include<cassert>
using namespace std;

Canvas::~Canvas()
{
	for (int i = 0; i < pixelsets.size(); i++) {
		delete pixelsets[i];
	}
}

Canvas::Canvas(const Canvas & B)
{
	if (this != &B) {
		color = B.color;
		pixelsets.clear();
		for (int i = 0; i < B.pixelsets.size(); i++) {
			//TODO:图元复制
			PixelSet *p=nullptr;
			switch (B.pixelsets[i]->type)
			{
			case LINE:
				p = new Line(*((Line*)B.pixelsets[i]));
				break;
			case POLYGON:
				p = new Polygon(*((Polygon*)B.pixelsets[i]));
				break;
			case ELLIPSE:
				p = new Ellipse(*((Ellipse*)B.pixelsets[i]));
				break;
			case CURVE:
				p = new Curve(*((Curve*)B.pixelsets[i]), *this);
				break;
			case FOLDLINE:
				p = new FoldLine(*((FoldLine*)B.pixelsets[i]));
				break;
			default:
				break;
			}
			if (p != nullptr)
				pixelsets.push_back(p);
		}
	}
}

const Canvas & Canvas::operator=(const Canvas & B)
{
	if (this != &B) {
		color = B.color;
		pixelsets.clear();
		for (int i = 0; i < B.pixelsets.size(); i++) {
			//TODO:图元复制
			PixelSet *p;
			switch (B.pixelsets[i]->type)
			{
			case LINE:
				p = new Line(*((Line*)B.pixelsets[i]));
				break;
			case POLYGON:
				p = new Polygon(*((Polygon*)B.pixelsets[i]));
				break;
			case ELLIPSE:
				p = new Ellipse(*((Ellipse*)B.pixelsets[i]));
				break;
			case CURVE:
				p = new Curve(*((Curve*)B.pixelsets[i]), *this);
				break;
			case FOLDLINE:
				p = new FoldLine(*((FoldLine*)B.pixelsets[i]));
				break;
			case CTRLPOINT:
				p = new CtrlPoint(*((CtrlPoint*)B.pixelsets[i]), *this);
				break;
			default:
				p = nullptr;
				break;
			}
			if (p!=nullptr)
				pixelsets.push_back(p);
		}
	}
	return *this;
}

void Canvas::setColor(int r, int g, int b)
{
	color.setRgb(r, g, b);
}

void Canvas::getIamge(QImage *image)
{
	clearPixelSet();//输出前，先清除无效图元
	image->fill(Qt::white);
	for (int i = 0; i < pixelsets.size(); i++) {
		pixelsets[i]->paint(image);
	}
}

void Canvas::getIamge_forSave(QImage * image)
{
	clearPixelSet();//输出前，先清除无效图元
	image->fill(Qt::white);
	for (int i = 0; i < pixelsets.size(); i++) {
		if (pixelsets[i]->type == LINE || pixelsets[i]->type == POLYGON || pixelsets[i]->type == ELLIPSE || pixelsets[i]->type == CURVE)
			pixelsets[i]->paint(image);
	}
}

void Canvas::translate(int id, int dx, int dy)
{
	for (size_t i = 0; i < pixelsets.size(); i++) {
		if (pixelsets[i]->id == id) {
			pixelsets[i]->translate(dx, dy);
			return;
		}
	}
}

void Canvas::rotate(int id, int x, int y, int r)
{
	for (auto i = pixelsets.begin(); i != pixelsets.end(); i++) {
		if ((*i)->id == id) {
			(*i)->rotate(x, y, r);
			return;
		}
	}
}

void Canvas::scale(int id, int x, int y, double s)
{
	for (auto i = pixelsets.begin(); i != pixelsets.end(); i++) {
		if ((*i)->id == id) {
			(*i)->scale(x, y, s);
			return;
		}
	}
}

void Canvas::clip(int id, int ix1, int iy1, int ix2, int iy2, ALGORITHM algorithm)
{
	int x1 = min(ix1, ix2); int x2 = max(ix1, ix2);
	int y1 = min(iy1, iy2); int y2 = max(iy1, iy2);
	for (auto i = pixelsets.begin(); i != pixelsets.end(); i++) {
		if ((*i)->id == id) {
			(*i)->clip(x1, y1, x2, y2, algorithm);
			return;
		}
	}
}

void Canvas::clipAll(int ix1, int iy1, int ix2, int iy2, ALGORITHM algorithm)
{
	int x1 = min(ix1, ix2); int x2 = max(ix1, ix2);
	int y1 = min(iy1, iy2); int y2 = max(iy1, iy2);
	for (auto i = pixelsets.begin(); i != pixelsets.end(); i++) {
		(*i)->clip(x1, y1, x2, y2, algorithm);
	}
}


int Canvas::getID(int x, int y)
{
	for (size_t i = 0; i < pixelsets.size(); i++) {
		int res=pixelsets[i]->getID(x, y);
		if (res != -1) return res;
	}
	return -1;
}

int Canvas::getType(int id)
{
	for (size_t i = 0; i < pixelsets.size(); i++) {
		if (pixelsets[i]->id == id) {
			return pixelsets[i]->type;
		}
	}
	return -1;
}

void Canvas::delID(int id)
{
	for (auto it = pixelsets.begin(); it != pixelsets.end();) {
		if ((*it)->id == id) {
			if ((*it)->type == CURVE) {
				delete (Curve*)(*it);
			}else{
				delete (*it);
			}
			it = pixelsets.erase(it);
			return;
		}
		else {
			++it;
		}
	}
	return;
}

PixelSet * Canvas::getPixelSet(int id)
{
	for (size_t i = 0; i < pixelsets.size(); i++) {
		if (pixelsets[i]->id == id) {
			return pixelsets[i];
		}
	}
	return nullptr;
}

void Canvas::clearPixelSet()
{
	for (auto it = pixelsets.begin(); it != pixelsets.end();) {
		if ((*it)->clear_flag == true) {
			delete (*it);
			it = pixelsets.erase(it);
		}
		else {
			++it;
		}
	}
	return;
}

void Canvas::drawLine(int id, int x1, int y1, int x2, int y2, ALGORITHM algorithm)
{
	
	PixelSet *p;
	switch (algorithm)
	{
	case DDA:
		p = new Line(x1, y1, x2, y2, "DDA");
		break;
	case BRESENHAM:
		p = new Line(x1, y1, x2, y2, "Bresenham");
		break;
	default:
		p = new Line(x1, y1, x2, y2, "DDA");
		break;
	}
	p->refresh();
	p->setID(id);
	p->setColor(CG_Painter::WColor);
	pixelsets.push_back(p);
}

void Canvas::drawPolygon(int id, const vector<Point> &vertexs, ALGORITHM algorithm)
{
	bool hasID = false;
	int keyIndex = -1;
	for (int i = 0; i < pixelsets.size(); i++) {
		if (pixelsets[i]->id == id) {
			hasID = true; keyIndex = i;
		}
	}
	if (hasID) { //已有，则重画
		delete pixelsets[keyIndex];
		if (algorithm == DDA)
			pixelsets[keyIndex] = new Polygon(vertexs, "DDA");
		else
			pixelsets[keyIndex] = new Polygon(vertexs, "Bresenham");
		pixelsets[keyIndex]->refresh();
		pixelsets[keyIndex]->setID(id);
		pixelsets[keyIndex]->setColor(color);
	}
	else {
		PixelSet *p;
		if (algorithm == DDA)
			p = new Polygon(vertexs, "DDA");
		else
			p = new Polygon(vertexs, "Bresenham");
		p->refresh();
		p->setID(id);
		p->setColor(CG_Painter::WColor);
		pixelsets.push_back(p);
	}
}

void Canvas::drawEllipse(int id, int x, int y, int rx, int ry)
{
	PixelSet *p = new Ellipse(x, y, rx, ry);
	p->refresh();
	p->setID(id);
	p->setColor(CG_Painter::WColor);
	pixelsets.push_back(p);
}

void Canvas::drawDotPoint(int id, int x, int y, int iwidth, QColor icolor)
{
	PixelSet *p = new DotPoint(x, y, iwidth, icolor);
	p->setID(id);
	pixelsets.push_back(p);
}

void Canvas::drawRectangle(int id, int x1, int y1, int x2, int y2, int iwidth, QColor icolor)
{
	PixelSet *p = new Rectangle(x1, y1, x2, y2, iwidth, icolor);
	p->setID(id);
	pixelsets.push_back(p);
}

void Canvas::drawCurve(int id, ALGORITHM algorithm, FoldLine *foldline)
{
	PixelSet *p = new Curve(algorithm, foldline);
	p->setID(id);
	p->setColor(CG_Painter::WColor);
	pixelsets.push_back(p);
}

FoldLine * Canvas::drawFoldLine(int id, const vector<Point>& vertexs)
{
	FoldLine *res = new FoldLine(vertexs);
	PixelSet *p = res;
	p->refresh();
	p->setID(id);
	pixelsets.push_back(p);
	return res;
}

void Canvas::drawCtrlPoint(int id, size_t index, FoldLine * foldline)
{
	PixelSet *p = new CtrlPoint(index, foldline);
	p->setID(id);
	pixelsets.push_back(p);
}
