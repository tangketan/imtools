window.MathJax = {
  tex: {
    inlineMath: [['$', '$'], ['\\(', '\\)']]
  },
  svg: {
    fontCache: 'global'
  }
};

(function () {
  var script = document.createElement('script');
  // cdn version: script.src="https://cdn.jsdelivr.net/npm/mathjax@3.0.0/es5/tex-chtml.js"
  script.src = './node_modules/mathjax/es5/tex-chtml.js';
  script.async = true;
  document.head.appendChild(script);
})();