/********************************************************************************
*  This file is part of CinoLib                                                 *
*  Copyright(C) 2016: Marco Livesu                                              *
*                                                                               *
*  The MIT License                                                              *
*                                                                               *
*  Permission is hereby granted, free of charge, to any person obtaining a      *
*  copy of this software and associated documentation files (the "Software"),   *
*  to deal in the Software without restriction, including without limitation    *
*  the rights to use, copy, modify, merge, publish, distribute, sublicense,     *
*  and/or sell copies of the Software, and to permit persons to whom the        *
*  Software is furnished to do so, subject to the following conditions:         *
*                                                                               *
*  The above copyright notice and this permission notice shall be included in   *
*  all copies or substantial portions of the Software.                          *
*                                                                               *
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
*  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE *
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       *
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      *
*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS *
*  IN THE SOFTWARE.                                                             *
*                                                                               *
*  Author(s):                                                                   *
*                                                                               *
*     Marco Livesu (marco.livesu@gmail.com)                                     *
*     http://pers.ge.imati.cnr.it/livesu/                                       *
*                                                                               *
*     Italian National Research Council (CNR)                                   *
*     Institute for Applied Mathematics and Information Technologies (IMATI)    *
*     Via de Marini, 6                                                          *
*     16149 Genoa,                                                              *
*     Italy                                                                     *
*********************************************************************************/
#ifndef CINO_ABSTRACT_POLYGON_MESH_H
#define CINO_ABSTRACT_POLYGON_MESH_H

#include <cinolib/meshes/abstract_mesh.h>
#include <cinolib/ipair.h>
#include <cinolib/symbols.h>

namespace cinolib
{

template<class M,
         class V,
         class E,
         class P>
class AbstractPolygonMesh : public AbstractMesh<M,V,E,P>
{
    protected:

        std::vector<std::vector<uint>> poly_triangles; // triangles covering each quad. Useful for
                                                       // robust normal estimation and rendering

    public:

        explicit AbstractPolygonMesh() : AbstractMesh<M,V,E,P>() {}
        virtual ~AbstractPolygonMesh() {}

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void load(const char * filename);
        void save(const char * filename) const;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void clear();
        void init(const std::vector<vec3d>             & verts,
                  const std::vector<std::vector<uint>> & polys);
        void init(      std::vector<vec3d>             & pos,       // vertex xyz positions
                        std::vector<vec3d>             & tex,       // vertex uv(w) texture coordinates
                        std::vector<vec3d>             & nor,       // vertex normals
                        std::vector<std::vector<uint>> & poly_pos,  // polygons with references to pos
                  const std::vector<std::vector<uint>> & poly_tex,  // polygons with references to tex
                  const std::vector<std::vector<uint>> & poly_nor,  // polygons with references to nor
                  const std::vector<Color>             & poly_col); // per polygon colors

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

                void update_p_tessellation(const uint pid);
        virtual void update_p_normal(const uint pid);
                void update_v_normal(const uint vid);
                void update_p_tessellations();
                void update_p_normals();
                void update_v_normals();
                void update_normals();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        int Euler_characteristic() const;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        virtual uint verts_per_poly(const uint pid) const { return this->polys.at(pid).size(); }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        const std::vector<uint> & adj_p2v(const uint pid) const { return this->polys.at(pid); }

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void operator+=(const AbstractPolygonMesh<M,V,E,P> & m);

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        std::vector<uint>  get_boundary_vertices() const;
        std::vector<ipair> get_boundary_edges() const;
        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        double mesh_volume() const;
        double mesh_area()   const;

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        void normalize_area();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

                bool              vert_is_saddle          (const uint vid, const int tex_coord = U_param) const;
                bool              vert_is_critical_p      (const uint vid, const int tex_coord = U_param) const;
                double            vert_area               (const uint vid) const;
        virtual double            vert_mass               (const uint vid) const;
                bool              vert_is_boundary        (const uint vid) const;
                void              vert_switch_id          (const uint vid0, const uint vid1);
                void              vert_remove             (const uint vid);
                void              vert_remove_unreferenced(const uint vid);
                uint              vert_add                (const vec3d & pos);
                std::vector<uint> vert_boundary_edges     (const uint vid) const;
                std::vector<uint> vert_ordered_vert_ring  (const uint vid) const;
                std::vector<uint> vert_ordered_poly_ring  (const uint vid) const;
                std::vector<uint> vert_ordered_edge_ring  (const uint vid) const;
                std::vector<uint> vert_ordered_edge_link  (const uint vid) const;
                void              vert_ordered_one_ring   (const uint          vid,
                                                           std::vector<uint> & v_ring,        // sorted list of adjacent vertices
                                                           std::vector<uint> & f_ring,        // sorted list of adjacent triangles
                                                           std::vector<uint> & e_ring,        // sorted list of edges incident to vid
                                                           std::vector<uint> & e_link) const; // sorted list of edges opposite to vid

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        bool   edge_is_manifold               (const uint eid, const bool boundaries_allowed = true) const;
        bool   edge_is_boundary               (const uint eid) const;
        double edge_crease_angle              (const uint eid) const;
        bool   edges_share_poly               (const uint eid1, const uint eid2) const;
        uint   edge_shared                    (const uint pid0, const uint pid1) const;
        void   edge_switch_id                 (const uint eid0, const uint eid1);
        uint   edge_add                       (const uint vid0, const uint vid1);
        void   edge_remove                    (const uint eid);
        void   edge_remove_unreferenced       (const uint eid);
        void   edge_mark_labeling_boundaries  ();
        void   edge_mark_color_discontinuities();
        void   edge_mark_boundaries           ();

        //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

              uint                 poly_vert_offset        (const uint pid, const uint vid) const;
              double               poly_angle_at_vert      (const uint pid, const uint vid, const int unit = RAD) const;
              double               poly_area               (const uint pid) const;
              double               poly_perimeter          (const uint pid) const;
              double               poly_mass               (const uint pid) const;
              int                  poly_shared             (const uint eid0, const uint eid1) const;
              bool                 polys_are_adjacent      (const uint pid0, const uint pid1) const;
              std::vector<uint>    polys_adjacent_along    (const uint pid, const uint vid0, const uint vid1) const;
              std::vector<uint>    polys_adjacent_along    (const uint pid, const uint eid) const;
              void                 poly_flip_winding_order (const uint pid);
              void                 poly_switch_id          (const uint pid0, const uint pid1);
              bool                 poly_is_boundary        (const uint pid) const;
              uint                 poly_add                (const std::vector<uint> & p);
              void                 poly_remove_unreferenced(const uint pid);
              void                 poly_remove             (const uint pid);
              void                 polys_remove            (const std::vector<uint> & pids);
              int                  poly_opposite_to        (const uint eid, const uint pid) const;
              bool                 poly_verts_are_CCW      (const uint pid, const uint curr, const uint prev) const;
              std::vector<vec3d>   poly_vlist              (const uint pid) const;
        const std::vector<uint>  & poly_tessellation       (const uint pid) const;
};

}

#ifndef  CINO_STATIC_LIB
#include "abstract_polygonmesh.cpp"
#endif

#endif //CINO_ABSTRACT_POLYGON_MESH_H
