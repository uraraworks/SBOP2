if (typeof _eglSwapInterval === "function") {
  _eglSwapInterval = function(display, interval) {
    void display;
    void interval;
    return 1;
  };
}
