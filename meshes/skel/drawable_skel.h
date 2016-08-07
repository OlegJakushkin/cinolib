/****************************************************************************
* Italian National Research Council                                         *
* Institute for Applied Mathematics and Information Technologies, Genoa     *
* IMATI-GE / CNR                                                            *
*                                                                           *
* Author: Marco Livesu (marco.livesu@gmail.com)                             *
*                                                                           *
* Copyright(C) 2016                                                         *
* All rights reserved.                                                      *
*                                                                           *
* This file is part of CinoLib                                              *
*                                                                           *
* CinoLib is free software; you can redistribute it and/or modify           *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
****************************************************************************/
#ifndef DRAWABLE_SKEL_H
#define DRAWABLE_SKEL_H

#include <cinolib/drawable_object.h>
#include <cinolib/meshes/skel/skel.h>

namespace cinolib
{

class DrawableSkel : public Skel, public DrawableObject
{
    public:

        enum
        {
            DRAW_SKEL            = 0x00000001,
            DRAW_POINTS          = 0x00000010,
            DRAW_FEATURE_POINTS  = 0x00000100,
            DRAW_BONES           = 0x00001000,
            DRAW_MAX_SPHERES     = 0x00010000,
            DRAW_STD_COLOR       = 0x00100000,
            DRAW_BONE_COLOR      = 0x01000000,
            DRAW_SELECTION_COLOR = 0x10000000
        };

        DrawableSkel();
        DrawableSkel(const Skel & s);
        DrawableSkel(const char * filename);
        DrawableSkel(const std::vector<double> & coords, const std::vector<int> & segs);

        void  draw()         const;
        vec3d scene_center() const { return bbox.center();     }
        float scene_radius() const { return bbox.diag() * 0.5; }

        void init();

        void set_draw_skel(bool b);
        void set_draw_bones(bool b);
        void set_draw_bone_points(bool b);
        void set_draw_feature_points(bool b);
        void set_draw_max_spheres(bool b);
        void set_enable_std_color();
        void set_enable_bone_color();
        void set_enable_selection_color();
        void set_std_leaf_color(float r, float g, float b);
        void set_std_bone_color(float r, float g, float b);
        void set_std_joint_color(float r, float g, float b);

        inline void set_std_leaf_color(float *rgb) { set_std_leaf_color(rgb[0], rgb[1], rgb[2]); }
        inline void set_std_bone_color(float *rgb) { set_std_bone_color(rgb[0], rgb[1], rgb[2]); }
        inline void set_std_joint_color(float *rgb) { set_std_joint_color(rgb[0], rgb[1], rgb[2]); }

        void update_bone_colors();

        inline const float * vertex_color (int vid) const { return &(v_colors[vid*3]); }
        inline const float * segment_color(int sid) const { return &(s_colors[sid*3]); }
        inline void bone_color(int bid, float * rgb) const
        {
            int first_segment = segment_bone(bid).front();
            const float * c = segment_color(first_segment);
            rgb[0] = c[0];
            rgb[1] = c[1];
            rgb[2] = c[2];
        }

        inline void set_vertex_color(int vid, float * color)
        {
            int vid_ptr = vid * 3;
            CHECK_BOUNDS(v_colors, vid_ptr+2);
            v_colors[vid_ptr + 0] = color[0];
            v_colors[vid_ptr + 1] = color[1];
            v_colors[vid_ptr + 2] = color[2];
        }

        inline void set_segment_color(int sid, float * color)
        {
            int sid_ptr = sid * 3;
            CHECK_BOUNDS(s_colors, sid_ptr+2);
            s_colors[sid_ptr + 0] = color[0];
            s_colors[sid_ptr + 1] = color[1];
            s_colors[sid_ptr + 2] = color[2];
        }

        inline void set_bone_thickness(float scalar)
        {
            bone_thickness = bbox.diag() * BONE_SCALAR() * scalar;
        }

        inline void set_sphere_radius(float scalar)
        {
            sphere_radius = bbox.diag() * BALL_SCALAR() * scalar;
        }

    private:

        inline float BONE_SCALAR() { return 0.004; }
        inline float BALL_SCALAR() { return 0.008; }

        int   draw_mode;
        float bone_thickness; // useful for rendering
        float sphere_radius;  // useful for rendering

        float std_bone_color[3];
        float std_leaf_color[3];
        float std_joint_color[3];

        std::vector<float> s_colors;
        std::vector<float> v_colors;        
};

}

#ifndef  CINO_STATIC_LIB
#include "drawable_skel.cpp"
#endif

#endif // DRAWABLE_SKEL_H