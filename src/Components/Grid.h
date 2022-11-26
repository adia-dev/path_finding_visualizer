//
// Created by abdoulayedia on 22.11.2022
//
#pragma once

#include "../Utilities/Singleton.h"
#include "../Core/classes.h"

#define DIRECTIONS                         \
    {                                      \
        {0, 1}, {1, 0}, {0, -1}, { -1, 0 } \
    }

namespace se
{
    enum class CellState
    {
        Unvisited,
        Visited,
        Near,
        Obstacle,
        Hole,
        Start,
        End,
        None,
        Count
    };

    struct Cell
    {

        Cell()
        {
            this->state = CellState::Unvisited;
            this->coords = {0, 0};
        }

        Cell(CellState state, const ImVec2 &coords)
        {
            this->state = state;
            this->coords = coords;
        }

        ImU32 GetColor() const
        {
            switch (this->state)
            {
            case CellState::Unvisited:
                return IM_COL32(255, 255, 255, 100);
            case CellState::Visited:
                return IM_COL32(100, 50, 217, 200);
            case CellState::Near:
                return IM_COL32(200, 50, 217, 150);
            case CellState::Obstacle:
                return IM_COL32(0, 0, 255, 255);
            case CellState::Hole:
                return IM_COL32(0, 0, 255, 255);
            case CellState::Start:
                return IM_COL32(255, 255, 0, 255);
            case CellState::End:
                return IM_COL32(255, 0, 0, 255);
            case CellState::None:
            case CellState::Count:
            default:
                return IM_COL32(0, 0, 0, 0);
            }

            return IM_COL32(0, 0, 0, 0);
        }

        ImVec2 coords;
        CellState state;
        bool isActive = true;
        float roundness = 0.0f;
    };

    class Grid
    {
    public:
        Grid();
        Grid(uint16_t width, uint16_t height);
        Grid(uint16_t width, uint16_t height, uint16_t cell_size);
        Grid(uint16_t width, uint16_t height, uint16_t cell_size, uint16_t cell_spacing);

        ~Grid();

        // Getters
        uint16_t GetWidth() const { return this->_width; }
        uint16_t GetHeight() const { return this->_height; }
        uint16_t GetCellSize() const { return this->_cell_size; }
        uint16_t GetCellCount() const { return this->_cell_count; }
        std::vector<std::vector<Cell>> GetCells() const { return this->_cells; }

        // Setters
        void SetWidth(uint16_t width) { this->_width = width; }
        void SetHeight(uint16_t height) { this->_height = height; }
        void SetCellSize(uint16_t cell_size) { this->_cell_size = cell_size; }
        void SetCellCount(uint16_t cell_count) { this->_cell_count = cell_count; }
        void SetCells(const std::vector<std::vector<Cell>> &cells) { this->_cells = cells; }

        // Loop Methods
        void Render();
        void Update(bool &is_running);

        // Mostly for public usage
        bool ResizeGrid(u_int16_t width, u_int16_t height);

    private:
        std::vector<std::vector<Cell>> _cells;
        std::vector<std::vector<bool>> _visited;
        std::queue<ImVec2> _queue;

        uint16_t _width = 0;
        uint16_t _height = 0;
        int _cell_size = 0;
        int _cell_count = 0;
        int _cell_spacing = 1;
        Cell *_start = nullptr;
        Cell *_end = nullptr;

        // Private Methods
        void InitCells();
        bool InitStartCell(u_int16_t x, u_int16_t y);
        bool InitEndCell(u_int16_t x, u_int16_t y);
        void ResetCells();
        void ClearQueue();
    };

}