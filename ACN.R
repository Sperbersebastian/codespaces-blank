library(igraph)
library(Matrix)

# Function to perform spectral clustering
spectral_clustering <- function(graph) {
  # Calculate the Laplacian matrix using sparse matrices
  L <- as.matrix(laplacian_matrix(graph, sparse = TRUE))
  
  # Compute the eigenvalues and eigenvectors (only the smallest two)
  eig <- eigen(L, symmetric = TRUE, only.values = FALSE)
  
  # Get the eigenvector corresponding to the second smallest eigenvalue
  corresponding_eigenvector <- eig$vectors[, 2]
  
  # Divide the nodes into two clusters based on the sign of the eigenvector elements
  cluster1 <- which(corresponding_eigenvector >= 0)
  cluster2 <- which(corresponding_eigenvector < 0)
  
  return(list(cluster1 = cluster1, cluster2 = cluster2))
}

# Function to calculate modularity
calculate_modularity <- function(g, clustering) {
  numc <- unique(clustering)
  m <- length(E(g))
  d <- degree(g)
  modularity <- 0
  
  for (i in numc) {
    indc <- which(clustering == i)
    e_internal <- 0
    sumd <- 0
    
    for (j in indc) {
      # Sum degrees of nodes in the cluster
      sumd <- sumd + d[j]
      
      # Count internal edges within the cluster
      neighbors <- neighbors(g, j)
      e_internal <- e_internal + sum(neighbors %in% indc)
    }
    
    # Every internal edge counted twice
    e_internal <- e_internal / 2
    
    # Update modularity
    modularity <- modularity + e_internal / m - (sumd / (2 * m))^2
  }
  
  return(modularity)
}

# Function to optimize clusters
optimize_clusters <- function(g, initial_clusters) {
  # Initialize clusters
  clusters <- initial_clusters
  cluster1 <- clusters$cluster1
  cluster2 <- clusters$cluster2
  
  # Create a clustering vector
  clustering <- rep(1, vcount(g))
  clustering[cluster2] <- 2
  
  # Current modularity
  current_modularity <- calculate_modularity(g, clustering)
  
  # Function to try moving a node and check modularity change
  try_move <- function(node, clustering, current_modularity) {
    # Create a copy of the current clustering
    new_clustering <- clustering
    
    # Move the node
    new_clustering[node] <- ifelse(new_clustering[node] == 1, 2, 1)
    
    # Calculate new modularity
    new_modularity <- calculate_modularity(g, new_clustering)
    
    if (new_modularity > current_modularity) {
      return(list(new_modularity, new_clustering))
    } else {
      return(list(current_modularity, NULL))
    }
  }
  
  # Iteratively move nodes between clusters to maximize modularity
  improvement <- TRUE
  while (improvement) {
    improvement <- FALSE
    
    # Try moving nodes from cluster1 to cluster2
    for (node in cluster1) {
      result <- try_move(node, clustering, current_modularity)
      if (!is.null(result[[2]])) {
        current_modularity <- result[[1]]
        clustering <- result[[2]]
        cluster1 <- which(clustering == 1)
        cluster2 <- which(clustering == 2)
        improvement <- TRUE
      }
    }
    
    # Try moving nodes from cluster2 to cluster1
    for (node in cluster2) {
      result <- try_move(node, clustering, current_modularity)
      if (!is.null(result[[2]])) {
        current_modularity <- result[[1]]
        clustering <- result[[2]]
        cluster1 <- which(clustering == 1)
        cluster2 <- which(clustering == 2)
        improvement <- TRUE
      }
    }
  }
  
  return(list(cluster1 = cluster1, cluster2 = cluster2))
}

# Example usage

g <- make_graph("zachary")

# Perform spectral clustering to initialize clusters
initial_clusters <- spectral_clustering(g)
print(initial_clusters)
# Calculate modularity of initial clustering
initial_clustering_vector <- rep(1, vcount(g))
initial_clustering_vector[initial_clusters$cluster2] <- 2
initial_modularity <- calculate_modularity(g, initial_clustering_vector)
cat("Initial Modularity:", initial_modularity, "\n")

# Optimize clusters
optimized_clusters <- optimize_clusters(g, initial_clusters)
print(optimized_clusters)
# Calculate modularity of optimized clustering
optimized_clustering_vector <- rep(1, vcount(g))
optimized_clustering_vector[optimized_clusters$cluster2] <- 2
optimized_modularity <- calculate_modularity(g, optimized_clustering_vector)
cat("Optimized Modularity:", optimized_modularity, "\n")
