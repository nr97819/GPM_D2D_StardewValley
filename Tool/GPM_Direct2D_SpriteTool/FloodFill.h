#pragma once

#include <conio.h>

#include <vector>
#include <wtypes.h>
using std::vector;

typedef struct POS
{
	int x;
	int y;

	POS()
	{
		x = 0;
		y = 0;
	}

	POS(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
}element;

void print_map(DWORD** _buffer, UINT _width, UINT _height)
{
	for (int j = 0; j < _height; ++j) {
		for (int i = 0; i < _width; ++i)
		{
			DWORD hex = *((*_buffer) + (j * _width) + i);
			BYTE alpha = static_cast<BYTE>((hex & 0xff'00'00'00) >> (8 * 3));

			//if (alpha == 0xff'ff'ff'ff)
			if (alpha == 0xff)
			{
				printf("1 ");
				//printf("%d ", );
			}
			//else if (*(map2 + (j * WIDTH) + i) == 0x00'00'00'00)
			else if (alpha == 0x00)
			{
				printf("0 ");
			}
			else
			{
				// 에러 발생
			}
		}
		printf("\n");
	}
	printf("\n");
}

void print_stack(const vector<element>& _stack)
{
	printf("Stack : ");
	if (_stack.empty())
	{
		printf("Empty");
	}
	else
	{
		//for (size_t i = 0; i <= _stack.size(); ++i)
		for (vector<element>::const_iterator cit = _stack.begin(); cit != _stack.end(); ++cit)
		{
			printf("(%d, %d) ", cit->x, cit->y);
		}
	}
	printf("\n");
}

D2D1_RECT_F FloodFill(DWORD** _buffer, UINT _width, UINT _height, UINT _firstX, UINT _firstY)
{
	// 초기화 X (이미 값 복사되어 들어있는 상태이므로)
	
	// 초기화 후, 첫 데이터 출력
	//print_map();

	// ===================
	D2D1_RECT_F result = {};
	result.left = _firstX;
	result.top = _firstY;
	result.right = _firstX;
	result.bottom = _firstY;

	// 시작
	vector<element> stack;
	stack.clear();

	stack.push_back(element(_firstX, _firstY));
	//Push(&to_visit, get_element(0, 0)); // start cell  (바깥 쪽 하얀색)
	// Push(&to_visit, get_element(3, 3)); // start cell (안 쪽 하얀색)

	while (!stack.empty())
	{
		element cell = stack.back();
		stack.pop_back();

		if ((*((*_buffer) + (cell.y * _width) + cell.x) & 0xff'00'00'00) == 0x00'00'00'00) // already visited
			continue;
		
		// ======== MAX / MIN ========
		if (result.top < cell.y)	result.top = cell.y;
		if (result.bottom > cell.y)	result.bottom = cell.y;
		if (result.left > cell.x)	
			result.left = cell.x;
		if (result.right < cell.x)	result.right = cell.x;
		// ===========================

		*((*_buffer) + (cell.y * _width) + cell.x) &= 0x00'ff'ff'ff; // 방문 처리

		// 현재 방문 중인 위치에서 갈 수 있는 곳들 스택에 넣기 (상하좌우)
		if (cell.y - 1 >= 0 && (*((*_buffer) + ((cell.y - 1) * _width) + cell.x) & 0xff'00'00'00) > 0x00'00'00'00) // up
			stack.push_back(element(cell.x, cell.y - 1));
		//Push(&to_visit, get_element(cell.i, cell.j - 1));

		if (cell.y + 1 < _height && (*((*_buffer) + ((cell.y + 1) * _width) + cell.x) & 0xff'00'00'00) > 0x00'00'00'00) // down
			stack.push_back(element(cell.x, cell.y + 1));
		//Push(&to_visit, get_element(cell.i, cell.j + 1));

		if (cell.x - 1 >= 0 && (*((*_buffer) + (cell.y * _width) + cell.x - 1) & 0xff'00'00'00) > 0x00'00'00'00) // left
			stack.push_back(element(cell.x - 1, cell.y));
		//Push(&to_visit, get_element(cell.i - 1, cell.j));

		if (cell.x + 1 < _width && (*((*_buffer) + (cell.y * _width) + cell.x + 1) & 0xff'00'00'00) > 0x00'00'00'00) // right
			stack.push_back(element(cell.x + 1, cell.y));
		//Push(&to_visit, get_element(cell.i + 1, cell.j));

		// 애니메이션 효과로 출력
		/*system("cls"); // #include <conio.h> 에서 지원하는, 콘솔창 지우는 명령어! (clear screen 의 약자) system(커맨드)
		print_stack(stack);
		printf("Cell : (%d, %d)\n", cell.x, cell.y);
		print_map(_buffer, _width, _height);
		int dummy = _getch(); // 사용자가 엔터 쳐야 다음으로 넘어가게끔*/
	}

	printf("Result:\n");
	print_map(_buffer, _width, _height);

	return result;
}