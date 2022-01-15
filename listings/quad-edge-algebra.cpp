    struct edge {
      size_t next;
      size_t data;
    };

    struct quad_edge {
      edge data[4];
    };

    vector<vertex>    vertices{};
    vector<quad_edge> quad_edges{};
    vector<size_t>    free_edges{};