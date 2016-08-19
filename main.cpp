#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "./inc/graph/graph.h"
#include "./inc/io/fileio.h"
#include "./inc/util/unionfind.h"
#include "./inc/visualization/render.h"

int main(int argc, char** args)
{
	std::string fname(args[1]);
	std::string vertfile(fname + ".vert");
	std::string facefile(fname + ".face");


	int patch_threshold = 8;
	if(argc > 2) patch_threshold = atoi( args[argc-1] );


	//Load molecular surface as generated from MSMS program
	Graph mesh_graph;
	FileIO::instance()->mesh_from_file(vertfile, facefile, mesh_graph);

	//Compute curvature for each point in mesh
	mesh_graph.compute_curvatures();

	//classify each point as convex/concave/flat
	mesh_graph.classify_points();

	//Get contiguous regions of convex/concave/flat points
	UnionFind uf( mesh_graph.size() );
	mesh_graph.segment_by_curvature(uf);

	//Extract feature points by expanding points until we
	//reach the borders
	std::vector<Patch> feature_points;
	mesh_graph.feature_points(uf, feature_points, patch_threshold);

	//visualize mesh
	//Render::instance()->draw_mesh( mesh_graph );

	//generate descriptors
	for(auto f = feature_points.begin(); f != feature_points.end(); ++f)
		f->compute_descriptor( mesh_graph.get_nodes() );

	//Experimento: compute o raio médio de um patch e a média de pontos
	//
	// A ideia é achar uma base do espaço centrada no ponto de interesse
	// descrito, com o y alinhado com a normal do ponto. Daí, precisamos
	// construir uma base. A daisy será centrado com o centro na origem,
	// deitado nos plano X-Z.
	// Projetamos então os pontos do patch no plano X-Z e medimos as coisas
	// a partir daí.
	// O raio da daisy coincide com a distância do ponto mais distante do centro
	// depois que projetamos o patch no plano X-Z. Esse raio deve ser
	// usado como informação no descritor final.
	//
	//
	// O descritor deve conter:
	// 	1) Flag Convexo/Côncavo: flag indicando se o patch é côncavo ou convexo.
	//	2) Raio da daisy: raio (inteiro? float?) da daisy usada para descrever essa região.
	//					  Essa informação codifica efetivamente o tamanho do patch.
	//	3) Distância do ponto ao daisy: essa informação é efetivamente sua coordenada y.
	//									Note que coordenadas negativas indicam que o patch
	//									é côncavo e coordenadas positivas que o patch é
	//									convexo. Em vez de armazenar o sinal, podemos
	//									simplesmente armazenar um flag dizendo se é convexo
	//									ou côncavo e armazenamos só o valor absoluto das coordenadas.
	//
	// A informação do raio é útil pois alguns patches de tamanhos diferentes podem ser encaixados
	// ainda assim: por exemplo, um patch côncavo de raio grande pode se encaixar com um patch convexo
	// de raio menor (mas não o contrário).
	//
	// Se as distâncias dos pontos à daisy são semelhantes, provavelmente o patch têm curvatura semelhante.
	//
	// Como achar a base? Temos a normal, precisamos de _ALGUM_ vetor perpendicular
	// à normal. Mas qual? O ideal seria escolher de uma forma consistente, de forma
	// que o mesmo patch rotacionado gerasse uma base rotacionada igual, de forma
	// a ganhar invariância à rotação.
	//
	// Jeito Naïve: UPDATE -> ISSO NÃO FAZ SENTIDO POR CAUSA DO Z NO VETOR FINAL!
	// 	ax + by + cz = 0;
	// 	x = y = 1, 	  a + b + cz = 0
	//				  a + b = - cz
	//				  (a + b)/z = -c
	//			  	  -(a+b)/z = c
	// 	Então, se a normal é o vetor N = [a b c], o vetor P = [1 1 -(a+b)/z].
	// 	PROBLEMA: a divisão por Z pode causar muito erro numérico!
	// 	PROBLEMA 2: não é invariante a rotação, porque o daisy vai pegar pontos
	// 				diferentes dependendo da orientação dele.
	//
	// OBS. 1: Se o patch projetado fosse perfeitamente circular, então não importa a orientação da base:
	//		   as distâncias computadas serão as mesmas. Isso significa que para orientar a base
	//		   é preciso haver alguma irregularidade na base.
	// Solução 1 : Orientar o segundo vetor a partir do vetor que vai do centro até o ponto mais distante.
	// Solução 2 : Calcular a caixa envoltória (não alinhada com os eixos) para o patch. Defina x como
	//				a dimensão maior da caixa e z como a dimensão menor. Isso deveria garantir invariância à rotação!
	//			   Mas como calcular a caixa envoltória rapidamente?
	// Solução 3 : Se em vez da caixa envoltória, usamos os autovetores com maiores autovalores? -> ANÁLISE DE COMPONENTE PRINCIPAL!
	// Solução 4 : Os componentes principais menores podem ser usados para guiar o tamanho da daisy
	// OBS. 2 : Ora, mas a Daisy em si não é necessária! Somente a distância ao plano já seria suficiente.
	return 0;
}