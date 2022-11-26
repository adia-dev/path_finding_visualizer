#include "Grid.h"

namespace se
{
    Grid::Grid()
        : _width(0), _height(0), _cell_size(0), _cell_count(0)
    {
        this->_cells = std::vector<std::vector<Cell>>();
    }

    Grid::Grid(uint16_t width, uint16_t height)
        : _width(width), _height(height), _cell_size(0), _cell_count(0)
    {
        this->_cells = std::vector<std::vector<Cell>>(this->_height, std::vector<Cell>(this->_width));

        // initialize the grid
        for (uint16_t i = 0; i < this->_height; i++)
        {
            for (uint16_t j = 0; j < this->_width; j++)
            {
                this->_cells[i][j] = Cell(CellState::Unvisited, ImVec2(j, i));
            }
        }
    }

    Grid::Grid(uint16_t width, uint16_t height, uint16_t cell_size)
        : _width(width), _height(height), _cell_size(cell_size), _cell_count(0)
    {
        this->_cells = std::vector<std::vector<Cell>>(this->_height, std::vector<Cell>(this->_width));

        // initialize the grid
        for (uint16_t i = 0; i < this->_height; i++)
        {
            for (uint16_t j = 0; j < this->_width; j++)
            {
                this->_cells[i][j] = Cell(CellState::Unvisited, ImVec2(j, i));
            }
        }
    }

    Grid::Grid(uint16_t width, uint16_t height, uint16_t cell_size, uint16_t cell_spacing)
        : _width(width), _height(height), _cell_size(cell_size), _cell_spacing(cell_spacing)
    {
        this->_cells = std::vector<std::vector<Cell>>(this->_height, std::vector<Cell>(this->_width));
        this->_visited = std::vector<std::vector<bool>>(this->_height, std::vector<bool>(this->_width, false));

        // initialize the grid
        for (uint16_t i = 0; i < this->_height; i++)
        {
            for (uint16_t j = 0; j < this->_width; j++)
            {
                this->_cells[i][j] = Cell(CellState::Unvisited, ImVec2(j, i));
            }
        }

        _queue.push(ImVec2(_height / 2, _width / 2));
        _end = std::make_unique<Cell>(_cells[_height - 1][_width - 1]);
        _end->state = CellState::End;
        _start = std::make_unique<Cell>(_cells[_height / 2][_width / 2]);
    }

    Grid::~Grid()
    {
    }

    void Grid::Render()
    {
        ImGui::Begin("Viewport");
        // Set next element is open by default
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        const float cell_spacing = ImGui::GetStyle().ItemSpacing.y;

        ImVec2 base_pos = ImGui::GetCursorScreenPos();
        size_t size = _cells.size();

        for (uint16_t i = 0; i < _height; i++)
        {
            for (uint16_t j = 0; j < _width; j++)
            {

                ImVec2 cell_p1(base_pos.x + j * (_cell_size + _cell_spacing), base_pos.y + i * (_cell_size + _cell_spacing));
                ImVec2 cell_p2(cell_p1.x + _cell_size, cell_p1.y + _cell_size);

                // Draw cell
                draw_list->AddRectFilled(cell_p1, cell_p2, _cells[i][j].GetColor());

                if (ImGui::IsMouseHoveringRect(cell_p1, cell_p2))
                {

                    if (ImGui::IsMouseClicked(0))
                    {
                        _cells[i][j].state = (_cells[i][j].state == CellState::Unvisited) ? CellState::Visited : CellState::Unvisited;
                    }
                    else if (ImGui::IsMouseClicked(1))
                    {
                        _cells[i][j].state = CellState::Obstacle;
                    }
                }
            }
        }

        ImGui::Dummy(ImVec2((float)(_width * (_cell_size + _cell_spacing)), (float)(_height * (_cell_size + _cell_spacing))));

        ImGui::End();
    }

    void Grid::Update()
    {
        if (_queue.empty() || _end->state == CellState::Visited)
            return;

        // BFS
        auto current = _queue.front();
        _queue.pop();

        // check if the current cell is the end cell
        if (current.x == _end->coords.x && current.y == _end->coords.y || _visited[current.y][current.x])
            return;

        _visited[current.y][current.x] = true;

        // add the current cell to the visited list

        if (current.x + 1 < _width)
        {
            Cell *next = &_cells[current.y][current.x + 1];
            if (next->state == CellState::Unvisited)
            {
                _queue.push(next->coords);
                next->state = CellState::Visited;
            }
        }

        if (current.x - 1 >= 0)
        {
            Cell *next = &_cells[current.y][current.x - 1];
            if (next->state == CellState::Unvisited)
            {
                _queue.push(next->coords);
                next->state = CellState::Visited;
            }
        }

        if (current.y + 1 < _height)
        {
            Cell *next = &_cells[current.y + 1][current.x];
            if (next->state == CellState::Unvisited)
            {
                _queue.push(next->coords);
                next->state = CellState::Visited;
            }
        }

        if (current.y - 1 >= 0)
        {
            Cell *next = &_cells[current.y - 1][current.x];
            if (next->state == CellState::Unvisited)
            {
                _queue.push(next->coords);
                next->state = CellState::Visited;
            }
        }
    }

}