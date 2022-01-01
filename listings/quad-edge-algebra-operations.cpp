struct quad_edge_algebra {
  // ...
  static constexpr size_t edge_type_mask =
    sizeof(quad_edge) / sizeof(edge) - 1;
  static constexpr size_t quad_edge_mask = 
    ~edge_type_mask;

  auto rot(size_t e, int n = 1) const -> size_t {
    const size_t t = e + n;
    return (e & quad_edge_mask) |
           (t & edge_type_mask);
  }

  auto onext(size_t e) const -> size_t { 
    return ((edge*)quad_edges.data() + e)->next;
  }
};