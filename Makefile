std:
	gcc svg_maker.c -o svg_maker -lreadline -lm
yaml:
	g++ svg_maker_yaml.cpp -o svg_maker -lyaml-cpp -lfmt
html:
	g++ svg_maker_html.cpp -o svg_maker -lyaml-cpp -lfmt
