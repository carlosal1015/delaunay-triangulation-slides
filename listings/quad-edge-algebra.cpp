struct quad_edge_algebra {
	struct edge {
    size_t next;
    size_t data;
  };

  struct quad_edge {
    edge data[4];
  };

  vector<quad_edge> quad_edges{};
  vector<size_t> free_edges{};
};