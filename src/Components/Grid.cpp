#include "Grid.h"
#include "../Utilities/Maths.h"

namespace se
{
    Grid::Grid()
        : _width(0), _height(0), _cell_size(0), _cell_count(0), _start(nullptr), _end(nullptr)
    {
        // Do nothing for now
    }

    Grid::Grid(uint16_t width, uint16_t height)
        : _width(width), _height(height), _cell_size(0), _cell_count(0)
    {
        InitCells();
        InitStartCell(0, 0);
        InitEndCell(_width - 1, _height - 1);
    }

    Grid::Grid(uint16_t width, uint16_t height, uint16_t cell_size)
        : _width(width), _height(height), _cell_size(cell_size), _cell_count(0)
    {
        InitCells();
        InitStartCell(0, 0);
        InitEndCell(_width - 1, _height - 1);
    }

    Grid::Grid(uint16_t width, uint16_t height, uint16_t cell_size, uint16_t cell_spacing)
        : _width(width), _height(height), _cell_size(cell_size), _cell_spacing(cell_spacing)
    {

        InitCells();
        InitStartCell(0, 0);
        InitEndCell(_width - 1, _height - 1);
    }

    Grid::~Grid()
    {
    }

    void Grid::InitCells()
    {
        if (_cells.size() != _height || (_cells.size() > 0 && _cells[0].size() != _width))
        {
            this->_cells = std::vector<std::vector<Cell>>(this->_height, std::vector<Cell>(this->_width));
            this->_visited = std::vector<std::vector<bool>>(this->_height, std::vector<bool>(this->_width, false));
        }

        ResetCells();

        _queue.push(ImVec2(0, 0));
        _s.push(ImVec2(0, 0));
    }

    bool Grid::InitStartCell(u_int16_t x, u_int16_t y)
    {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
        {
            Logger::Logln("Could not set the start cell to \n\tx: " + std::to_string(x) + "\n\ty: " + std::to_string(y), Colors::Red);
            return false;
        }

        _start = &_cells[y][x];
        _start->state = CellState::Start;

        _queue.push(ImVec2(x, y));
        _s.push(ImVec2(x, y));

        return true;
    }

    bool Grid::InitEndCell(u_int16_t x, u_int16_t y)
    {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
        {
            Logger::Logln("Could not set the end cell to \n\tx: " + std::to_string(x) + "\n\ty: " + std::to_string(y), Colors::Red);
            return false;
        }

        _end = &_cells[y][x];
        _end->state = CellState::End;

        return true;
    }

    void Grid::ResetCells()
    {
        for (uint16_t i = 0; i < this->_height; i++)
        {
            for (uint16_t j = 0; j < this->_width; j++)
            {
                this->_cells[i][j] = Cell(CellState::Unvisited, ImVec2(j, i));
                this->_visited[i][j] = false;
            }
        }
    }

    void Grid::ClearQueue()
    {
        while (!_queue.empty())
            _queue.pop();

        while (!_s.empty())
            _s.pop();
    }

    bool Grid::ResizeGrid(u_int16_t width, u_int16_t height)
    {
        if (width < 0 || height < 0)
            return false;

        this->_width = width;
        this->_height = height;

        this->_cells.resize(height, std::vector<Cell>(width));
        this->_visited.resize(height, std::vector<bool>(width, false));

        ResetCells();

        return true;
    }

    void Grid::Render()
    {
        ImGui::Begin("Viewport");

        // Set next element is open by default
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImVec2 base_pos = ImGui::GetCursorScreenPos();
        size_t size = _cells.size();

        for (uint16_t i = 0; i < _height; i++)
        {
            for (uint16_t j = 0; j < _width; j++)
            {

                ImVec2 cell_p1(base_pos.x + j * (_cell_size + _cell_spacing), base_pos.y + i * (_cell_size + _cell_spacing));
                ImVec2 cell_p2(cell_p1.x + _cell_size, cell_p1.y + _cell_size);

                Cell *current_cell = &_cells[i][j];

                ImU32 cellColor = current_cell->GetColor();

                if (ImGui::IsMouseHoveringRect(cell_p1, cell_p2))
                {
                    cellColor = IM_COL32(255, 255, 255, 210);

                    if (ImGui::IsKeyPressed(ImGuiKey_LeftShift))
                    {
                        current_cell->state = CellState::Obstacle;
                    }

                    if (ImGui::IsKeyPressed(ImGuiKey_E))
                    {
                        if (_end != nullptr)
                            _end->state = CellState::Unvisited;

                        InitEndCell(j, i);
                    }

                    if (ImGui::IsKeyPressed(ImGuiKey_S))
                    {
                        ResetCells();
                        ClearQueue();

                        InitStartCell(j, i);
                    }
                }

                draw_list->AddRectFilled(cell_p1, cell_p2, cellColor, current_cell->roundness);
            }
        }

        ImGui::Dummy(ImVec2((float)(_width * (_cell_size + _cell_spacing)), (float)(_height * (_cell_size + _cell_spacing))));

        ImGui::End();
    }

    void Grid::Update(bool &is_running)
    {

        if (_DFS_selected)
        {
            if (!is_running || _s.empty() || _end->state == CellState::Visited)
                return;

            // DFS
            ImVec2 current = _s.top();
            _s.pop();

            if (_cells[current.y][current.x].state == CellState::End)
            {
                _end->state = CellState::Visited;
                std::cout << "Found the end!" << std::endl;
                ClearQueue();
                is_running = false;
            }

            if (_visited[current.y][current.x])
                return;

            if (_cells[current.y][current.x].state == CellState::Unvisited)
                _cells[current.y][current.x].state = CellState::Visited;

            _cells[current.y][current.x].state = CellState::Visited;
            _visited[current.y][current.x] = true;

            if (current.x > 0 && !_visited[current.y][current.x - 1] && _cells[current.y][current.x - 1].state != CellState::Obstacle)
            {
                Cell *next = &_cells[current.y][current.x - 1];
                if (next->state == CellState::Unvisited)
                    next->state = CellState::Near;
                _s.push(ImVec2(current.x - 1, current.y));
            }

            if (current.x < _width - 1 && !_visited[current.y][current.x + 1] && _cells[current.y][current.x + 1].state != CellState::Obstacle)
            {
                Cell *next = &_cells[current.y][current.x + 1];
                if (next->state == CellState::Unvisited)
                    next->state = CellState::Near;
                _s.push(ImVec2(current.x + 1, current.y));
            }

            if (current.y > 0 && !_visited[current.y - 1][current.x] && _cells[current.y - 1][current.x].state != CellState::Obstacle)
            {
                Cell *next = &_cells[current.y - 1][current.x];
                if (next->state == CellState::Unvisited)
                    next->state = CellState::Near;
                _s.push(ImVec2(current.x, current.y - 1));
            }

            if (current.y < _height - 1 && !_visited[current.y + 1][current.x] && _cells[current.y + 1][current.x].state != CellState::Obstacle)
            {
                Cell *next = &_cells[current.y + 1][current.x];
                if (next->state == CellState::Unvisited)
                    next->state = CellState::Near;
                _s.push(ImVec2(current.x, current.y + 1));
            }
        }
        else
        {

            if (!is_running || _queue.empty() || _end->state == CellState::Visited)
                return;

            // BFS
            auto current = _queue.front();
            _queue.pop();

            if (_visited[current.y][current.x])
                return;

            if (_cells[current.y][current.x].state == CellState::End)
            {
                is_running = false;
                std::cout << "Found the end!" << std::endl;
                ClearQueue();
                return;
            }

            _visited[current.y][current.x] = true;

            if (_cells[current.y][current.x].state == CellState::Obstacle)
                return;

            if (_cells[current.y][current.x].state != CellState::Start || _cells[current.y][current.x].state != CellState::End)
                _cells[current.y][current.x].state = CellState::Visited;

            if (current.x + 1 < _width)
            {
                Cell *next = &_cells[current.y][current.x + 1];
                if (next->state == CellState::Unvisited)
                    next->state = CellState::Near;

                _queue.push(next->coords);
            }

            if (current.x - 1 >= 0)
            {
                Cell *next = &_cells[current.y][current.x - 1];
                if (next->state == CellState::Unvisited)
                    next->state = CellState::Near;

                _queue.push(next->coords);
            }

            if (current.y + 1 < _height)
            {
                Cell *next = &_cells[current.y + 1][current.x];
                if (next->state == CellState::Unvisited)
                    next->state = CellState::Near;

                _queue.push(next->coords);
            }

            if (current.y - 1 >= 0)
            {
                Cell *next = &_cells[current.y - 1][current.x];
                if (next->state == CellState::Unvisited)
                    next->state = CellState::Near;

                _queue.push(next->coords);
            }
        }
    }
}