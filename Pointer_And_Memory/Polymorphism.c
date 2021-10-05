#include <stdio.h>
#include <stdlib.h>

typedef void (*fptrSet)(void*, int);
typedef int (*fptrGet)(void*);
typedef void (*fptrDisplay)();

typedef struct _functions{
	fptrSet setX;
	fptrGet getX;
	fptrSet setY;
	fptrGet getY;
	fptrDisplay display;
}vFunctions;

typedef struct _shape{
	vFunctions functions;
	int x;
	int y;
}Shape;

void shapeDisplay(Shape *shape){
	printf("Shape\n");
}
void shapeSetX(Shape *shape, int x){
	shape->x = x;
}
void shapeSetY(Shape *shape, int y){
	shape->y = y;
}
int shapeGetX(Shape *shape){
	return shape->x;
}
int shapeGetY(Shape *shape){
	return shape->y;
}
Shape* getShapeInstance(){
	Shape *shape = (Shape*)malloc(sizeof(Shape));
	shape->functions.display = shapeDisplay;
	shape->functions.setX = shapeSetX;
	shape->functions.getX = shapeGetX;
	shape->functions.setY = shapeSetY;
	shape->functions.getY = shapeGetY;
	shape->x = 100;
	shape->y = 100;
	return shape;
}

typedef struct _rectangle{
	Shape base;
	int width;
	int height;
}Rectangle;

void rectangleDisplay(Rectangle *rectangle){
	printf("Rectangle\n");
}
void rectangleSetX(Rectangle *rectangle, int x){
	rectangle->base.x = x;
}
void rectangleSetY(Rectangle *rectangle, int y){
	rectangle->base.y = y;
}
int rectangleGetX(Rectangle *rectangle){
	return rectangle->base.x;
}
int rectangleGetY(Rectangle *rectangle){
	return rectangle->base.y;
}
Shape* getRectangleInstance(){
	Rectangle *rectangle = (Rectangle*)malloc(sizeof(Rectangle));
	rectangle->base.functions.display = rectangleDisplay;
	rectangle->base.functions.setX = rectangleSetX;
	rectangle->base.functions.getX = rectangleGetX;
	rectangle->base.functions.setY = rectangleSetY;
	rectangle->base.functions.getY = rectangleGetY;
	rectangle->base.x = 100;
	rectangle->base.y = 100;
	rectangle->height = 300;
	rectangle->width = 500;
	return rectangle;
}

int main(){
	Shape *sptr = getShapeInstance();
	sptr->functions.setX(sptr, 35);
	sptr->functions.display();
	printf("%d\n", sptr->functions.getX(sptr));
	free(sptr);
	
	Rectangle *rptr = getRectangleInstance();
	rptr->base.functions.setX(rptr, 35);
	rptr->base.functions.display();
	printf("%d\n", rptr->base.functions.getX(rptr));
	printf("\n");
	
	Shape *shapes[3];
	shapes[0] = getShapeInstance();
	shapes[0]->functions.setX(shapes[0], 35);
	shapes[1] = getRectangleInstance();
	shapes[1]->functions.setX(shapes[1], 45);
	shapes[2] = getShapeInstance();
	shapes[2]->functions.setX(shapes[2], 55);
	int i;
	for(i = 0; i < 3; i++){
		shapes[i]->functions.display();
		printf("%d\n", shapes[i]->functions.getX(shapes[i]));
	}
	system("PAUSE");
	return 0;
}

