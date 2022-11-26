#include "Grid.h"
#include "../Utilities/Maths.h"

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

        // initialize the grid

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

        _queue.push(ImVec2(0, 0));
        _end = std::make_unique<Cell>(_cells[_height - 1][_width - 1]);
        _end->state = CellState::End;
        _start = std::make_unique<Cell>(_cells[0][0]);
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

                Cell *current_cell = &_cells[i][j];

                if (current_cell->state == CellState::End)
                {
                    _is_running = false;
                    std::cout << "Found the end!" << std::endl;
                }

                // Draw cell
                ImU32 cellColor = current_cell->GetColor();

                // lerp the roundness of the corners based on the cell state to 0
                // if (current_cell->state == CellState::Visited)
                // current_cell->roundness = Maths::lerp(current_cell->roundness, 7.5f, 0.01f);

                if (ImGui::IsMouseHoveringRect(cell_p1, cell_p2))
                {
                    cellColor = IM_COL32(255, 255, 255, 250);

                    // if hovered lerp the roundness of the cell to 2.0f

                    if (ImGui::IsMouseClicked(0))
                    {
                        current_cell->state = (current_cell->state == CellState::Unvisited) ? CellState::Visited : CellState::Unvisited;
                    }

                    if (ImGui::IsKeyPressed(ImGuiKey_LeftShift))
                    {
                        current_cell->state = CellState::Obstacle;
                    }

                    if (ImGui::IsKeyPressed(ImGuiKey_E))
                    {
                        if (_end != nullptr)
                            _end->state = CellState::Unvisited;
                        _end = std::make_unique<Cell>(_cells[i][j]);
                        _end->state = CellState::End;
                        std::cout << "End: " << _end->coords.x << ", " << _end->coords.y << std::endl;
                    }

                    if (ImGui::IsKeyPressed(ImGuiKey_S))
                    {
                        // clear the queue
                        while (!_queue.empty())
                            _queue.pop();

                        // reset the state of all the cells
                        for (uint16_t i = 0; i < _height; i++)
                        {
                            for (uint16_t j = 0; j < _width; j++)
                            {
                                if (_cells[i][j].state == CellState::Obstacle || _cells[i][j].state == CellState::End || _cells[i][j].state == CellState::Start)
                                    continue;
                                _cells[i][j].state = CellState::Unvisited;
                                _visited[i][j] = false;
                            }
                        }

                        _queue.push(ImVec2(j, i));
                        _start = std::make_unique<Cell>(_cells[i][j]);
                        _start->state = CellState::Start;
                    }
                }

                // depending on the distance from the middle of the _cells, adjust the alpha value
                // this will make the _cells fade out as they get further away from the middle
                float alpha = Maths::lerp(0.0f, 255.0f, 1.0f - (std::abs((float)_height / 2.0f - (float)i) / (float)_height));

                // cellColor |= ((ImU32)(alpha) << IM_COL32_R_SHIFT);

                draw_list->AddRectFilled(cell_p1, cell_p2, cellColor, current_cell->roundness);

                // draw_list->AddRectFilled(cell_p1, cell_p2, cellColor, current_cell->roundness);
            }
        }

        ImGui::Dummy(ImVec2((float)(_width * (_cell_size + _cell_spacing)), (float)(_height * (_cell_size + _cell_spacing))));

        ImGui::End();
    }

    void Grid::Update()
    {

        // // if I press the spacebar, then I want to start the algorithm
        if (ImGui::IsKeyReleased(ImGuiKey_Space))
        {
            this->_is_running = !this->_is_running;
        }

        if (!_is_running || _queue.empty() || _end->state == CellState::Visited)
            return;

        // BFS
        auto current = _queue.front();
        _queue.pop();

        // check if the current cell is the end cell
        if (current.x == _end->coords.x && current.y == _end->coords.y || _visited[current.y][current.x])
            return;

        _visited[current.y][current.x] = true;
        if (_cells[current.y][current.x].state != CellState::Start || _cells[current.y][current.x].state != CellState::End)
            _cells[current.y][current.x].state = CellState::Visited;

        if (_cells[current.y][current.x].state == CellState::End)
        {
            _is_running = false;
            std::cout << "Found the end!" << std::endl;
            return;
        }

        // add the current cell to the visited list

        if (current.x + 1 < _width)
        {
            Cell *next = &_cells[current.y][current.x + 1];

            _queue.push(next->coords);
        }

        if (current.x - 1 >= 0)
        {
            Cell *next = &_cells[current.y][current.x - 1];

            _queue.push(next->coords);
        }

        if (current.y + 1 < _height)
        {
            Cell *next = &_cells[current.y + 1][current.x];

            _queue.push(next->coords);
        }

        if (current.y - 1 >= 0)
        {
            Cell *next = &_cells[current.y - 1][current.x];

            _queue.push(next->coords);
        }
    }

}