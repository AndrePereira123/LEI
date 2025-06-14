document.addEventListener('DOMContentLoaded', function() {
  const categoriaFilter = document.getElementById('categoriaFilter');
  const itemsContainer = document.getElementById('itemsContainer');
  const loadingIndicator = document.getElementById('loadingIndicator');
  
  let allPosts = [];
  
  function initializePosts() {
    const postElements = document.querySelectorAll('.contedor-de-1-post');
    
    allPosts = Array.from(postElements).map(function(postElement) {
      // Extrair classificadores do post
      const classificadorElements = postElement.querySelectorAll('.w3-tag');
      const classificadores = Array.from(classificadorElements).map(function(tag) {
        return tag.textContent.trim().toLowerCase();
      });
      
      const titulo = postElement.querySelector('h2') ? 
        postElement.querySelector('h2').textContent.trim() : '';
      
      return {
        element: postElement,
        classificadores: classificadores,
        titulo: titulo,
        html: postElement.outerHTML
      };
    });
    
    console.log('📦 Posts inicializados:', allPosts.length);
    console.log('🏷️ Classificadores encontrados:', 
      [...new Set(allPosts.flatMap(p => p.classificadores))].sort()
    );
    
    allPosts.forEach((post, index) => {
      console.log(`Post ${index + 1}: "${post.titulo}" - Classificadores: [${post.classificadores.join(', ')}]`);
    });
  }

  initializePosts();

  // event listener para o filtro de categoria
  categoriaFilter.addEventListener('change', function() {
    const selectedCategory = this.value.toLowerCase().trim();
    console.log('🔍 Filtro selecionado:', selectedCategory || 'todos');
    
    // Mostrar loading
    if (loadingIndicator) {
      loadingIndicator.style.display = 'block';
    }
    itemsContainer.style.opacity = '0.5';
    
    setTimeout(function() {
      try {
        let filteredPosts;
        
        if (!selectedCategory) {
          // sem filtro - mostrar todos os posts
          filteredPosts = allPosts;
          console.log('📋 Mostrando todos os posts:', filteredPosts.length);
        } else {
          // com filtro - filtrar posts por categoria
          filteredPosts = allPosts.filter(function(post) {
            const hasCategory = post.classificadores.includes(selectedCategory);
            console.log(`🔎 Post "${post.titulo}": classificadores=[${post.classificadores.join(', ')}], inclui "${selectedCategory}": ${hasCategory}`);
            return hasCategory;
          });
          
          console.log(`Filtrados ${filteredPosts.length} de ${allPosts.length} posts para categoria "${selectedCategory}"`);
        }

        // atualizar display dos posts
        updatePostsDisplay(filteredPosts);
        
      } catch (error) {
        console.error('Erro ao filtrar posts:', error);
        showErrorMessage('Erro ao filtrar posts localmente.');
      } finally {
        if (loadingIndicator) {
          loadingIndicator.style.display = 'none';
        }
        itemsContainer.style.opacity = '1';
      }
    }, 100); 
  });
  
  // funcao para atualizar a exibição dos posts filtrados
  function updatePostsDisplay(posts) {
    if (!posts || posts.length === 0) {
      itemsContainer.innerHTML = `
        <div class="w3-panel w3-yellow w3-leftbar w3-border-yellow w3-pale-yellow">
          <h3 class="w3-text-black">📭 Nenhum item encontrado</h3>
          <p class="w3-text-black">Não existem items para o filtro selecionado.</p>
        </div>
      `;
      return;
    }

    // gerar HTML dos posts filtrados
    const htmlContent = posts.map(function(post) {
      return post.html;
    }).join('');
    
    itemsContainer.innerHTML = htmlContent;

    console.log(`Exibindo ${posts.length} posts`);
  }

  // função para mostrar erro
  function showErrorMessage(message) {
    itemsContainer.innerHTML = `
      <div class="w3-panel w3-red w3-leftbar w3-border-red">
        <h3 class="w3-text-white">⚠️ Erro</h3>
        <p class="w3-text-light-grey">${message}</p>
      </div>
    `;
  }

  // função para refresh dos posts (caso novos posts sejam adicionados dinamicamente)
  window.refreshPostsFilter = function() {
    console.log('🔄 Refreshing posts filter...');
    initializePosts();
    categoriaFilter.dispatchEvent(new Event('change')); // Re-aplicar filtro atual
  };

  // função para mostrar estatísticas
  window.showFilterStats = function() {
    const stats = {};
    allPosts.forEach(function(post) {
      post.classificadores.forEach(function(cat) {
        stats[cat] = (stats[cat] || 0) + 1;
      });
    });
    
    console.log('Estatísticas de classificadores:');
    console.table(stats);
    return stats;
  };

  // testar filtro
  window.testFilter = function(categoria) {
    console.log('Testando filtro para categoria:', categoria);
    categoriaFilter.value = categoria;
    categoriaFilter.dispatchEvent(new Event('change'));
  };
});
