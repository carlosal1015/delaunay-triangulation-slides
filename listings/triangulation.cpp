struct triangulation : quad_edge_algebra {
  triangulation(const vector<point>& input) {
    points = input;
    sort(points);
    divide_and_conquer(0, points.size());
  }

  auto divide_and_conquer(size_t first, size_t last) -> pair<size_t, size_t> {
    const auto count = last - first;
    if (count == 2) {
      make_edge();
      return;
    }

    if (count == 3) {
      make_edge_or_triangle();
      return;
    }

    const auto mid = first + count / 2;
    const auto [lo, li] = divide_and_conquer(first, mid);
    const auto [ri, ro] = divide_and_conquer(mid, last);

    const auto baseline = make_lower_convex_hull_edge(li, ri);
    merge();
    return {lo, ro};
  }

  void make_edge() {
    auto a = new_edge();
    set_onode(a, first);
    set_dnode(a, first + 1);
    return {a, sym(a)};
  }

  auto make_edge_or_triangle() {
    auto a = new_edge();
    auto b = new_edge();
    splice(sym(a), b);
    set_onode(a, first + 0);
    set_dnode(a, first + 1);
    set_onode(b, first + 1);
    set_dnode(b, first + 2);

    if (ccw(first + 0, first + 1, first + 2)) {
      auto c = connection(b, a);
      return {a, sym(b)};
    } else if (ccw(first + 0, first + 2, first + 1)) {
      auto c = connection(b, a);
      return {sym(c), c};
    } else
      return {a, sym(b)};
  }

  void make_lower_convex_hull_edge(size_t& li, size_t& ri) {
    while (true) {
      if (left_of(onode(ri), li))
        li = lnext(li);
      else if (right_of(onode(li), ri))
        ri = rprev(ri);
      else
        break;
    }

    auto baseline = connection(sym(ri), li);
    if (onode(li) == onode(ldo))
      ldo = sym(baseline);
    if (onode(ri) == onode(rdo))
      rdo = baseline;
  }

  void merge() {
    while (true) {
      get_left_and_right_candidate();
      remove_left_edges(baseline, lc);
      remove_right_edges(baseline, rc);
      if (!valid(lc) && !valid(rc)) break;
      baseline = insert_cross_edge(baseline, lc, rc);
    }
  }

  void remove_left_edges(size_t baseline, size_t& l) {
    if (valid(l)) {
      while (valid(onext(l)) &&
             in_circle(dnode(baseline), onode(baseline), dnode(l),
                       dnode(onext(l)))) {
        auto t = onext(l);
        remove(l);
        l = t;
      }
    }
  }

  void remove_right_edges(size_t baseline, size_t& r) {
    if (valid(r)) {
      while (valid(oprev(r)) &&
             in_circle(dnode(baseline), onode(baseline), dnode(r),
                       dnode(oprev(r)))) {
        auto t = oprev(r);
        remove(r);
        r = t;
      }
    }
  }

  auto insert_cross_edge(size_t baseline, size_t l, size_t r) {
    if (!valid(l) ||
        (valid(r) &&
         in_circle(dnode(l), onode(l), onode(r), dnode(r))))
      baseline = connection(r, sym(baseline));
    else
      baseline = connection(sym(baseline), sym(l));
    return baseline;
  }

  vector<point> points{};
};