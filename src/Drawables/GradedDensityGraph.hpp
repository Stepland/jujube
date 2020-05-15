#pragma once

#include <array>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../Data/Chart.hpp"
#include "../Data/GradedNote.hpp"
#include "../Data/Song.hpp"
#include "../Toolkit/AffineTransform.hpp"
#include "../Toolkit/Cache.hpp"
#include "../Toolkit/SFMLHelpers.hpp"
#include "DensityGraph.hpp"

namespace Drawables {
    enum class DensityLineGrade {
        Perfect,
        Great,
        ComboBreak,
        NonGraded
    };

    DensityLineGrade judgement_to_density_line_grade(Data::Judgement judge);
    DensityLineGrade merge_grades(DensityLineGrade current, DensityLineGrade _new);
    sf::Color grade_to_color(DensityLineGrade grade);

    struct GradedDensity {
        GradedDensity() = default;
        GradedDensity(unsigned int t_density) : density(t_density) {};
        
        DensityLineGrade grade = DensityLineGrade::NonGraded;
        unsigned int density = 0;
        // iterator to the first vertex of the corresponding column
        std::vector<sf::Vertex>::iterator first_vertex;
    };

    // helper functions for GradedDensityGraph
    Toolkit::AffineTransform<float> get_seconds_to_column_transform_from_notes(const Data::Chart& chart);
    Toolkit::AffineTransform<float> get_seconds_to_column_transform(const Data::SongDifficulty& sd);

    class GradedDensityGraph : public sf::Drawable, public sf::Transformable {
    public:
        explicit GradedDensityGraph(const DensityGraph& density_graph, const Data::SongDifficulty& sd);
        // Set verticies colors for density columns that have already been played
        void update(const sf::Time& music_time);
        // Update stored grades according to the recieved judgement
        void update_grades(const Data::Judgement& judge, const sf::Time& timing);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::array<GradedDensity, 115> m_densities;
        std::vector<sf::Vertex> m_vertex_array;
        std::array<GradedDensity, 115>::iterator first_non_played_density;
        Toolkit::AffineTransform<float> m_seconds_to_column;
    };
}

namespace Toolkit {
    template<>
    void set_origin_normalized(Drawables::GradedDensityGraph& s, float x, float y);
}