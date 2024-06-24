# file_path <- "/workspaces/codespaces-blank/ACN/Ex_11/ExpMatAra.txt"

library(Hmisc)
library(pheatmap)
library(igraph)

# Reading the gene expression profiles
exp_mat <- read.table("/workspaces/codespaces-blank/ACN/Ex_11/ExpMatAra.txt", header = TRUE, sep = "\t", row.names = 1)

# Reading the annotations for genes of interest
annotations <- read.table("/workspaces/codespaces-blank/ACN/Ex_11/Annotations_for_genes_of_interest.txt", header = TRUE, sep = "\t")

# Custom correlation function
custom_correlation <- function(mat) {
  n <- ncol(mat)
  cor_mat <- matrix(0, n, n)
  for (i in 1:n) {
    for (j in 1:n) {
      cor_mat[i, j] <- cor(mat[, i], mat[, j])
    }
  }
  return(cor_mat)
}

# Calculating correlation using custom function
custom_cor <- custom_correlation(exp_mat)

# Calculating correlation using rcorr function
rcorr_res <- rcorr(as.matrix(exp_mat))
hmisc_cor <- rcorr_res$r

# Function to calculate FDR corrected p-values
fdr_correction <- function(p_values) {
  p_adjusted <- p.adjust(p_values, method = "fdr")
  return(p_adjusted)
}

# Extracting p-values from rcorr result
p_values <- rcorr_res$P

# Correct reshaping of FDR corrected p-values
p_values_vector <- p_values[lower.tri(p_values)]
fdr_p_values_vector <- fdr_correction(p_values_vector)

# Create a full matrix of FDR corrected p-values
fdr_p_values <- matrix(NA, nrow = nrow(p_values), ncol = ncol(p_values))
fdr_p_values[lower.tri(fdr_p_values)] <- fdr_p_values_vector
fdr_p_values <- t(fdr_p_values)
fdr_p_values[lower.tri(fdr_p_values)] <- fdr_p_values_vector

# Identifying significantly co-expressed genes
significant_indices <- which(fdr_p_values < 0.05, arr.ind = TRUE)

# Extract unique gene names from the significant indices
significant_genes <- unique(c(rownames(significant_indices), colnames(significant_indices)))

# Ensure genes are available in the expression matrix
significant_genes <- significant_genes[significant_genes %in% colnames(exp_mat)]

# Subset the expression matrix for significant genes
if (length(significant_genes) < 2) {
  stop("Not enough significant genes found for clustering.")
}
significant_exp_mat <- exp_mat[, significant_genes]

# Calculating distance matrix
dist_mat <- dist(t(exp_mat))

# Perform clustering only if there are at least 2 samples
if (ncol(significant_exp_mat) >= 2) {
  # Custom hierarchical clustering function (using complete linkage)
  custom_hclust <- function(d) {
    hclust(d, method = "complete")
  }

  # Performing hierarchical clustering using custom function
  custom_hc <- custom_hclust(dist(t(significant_exp_mat)))

  # Performing hierarchical clustering using hclust
  hclust_res <- hclust(dist(t(significant_exp_mat)), method = "complete")

  # Plotting the dendrograms
  par(mfrow = c(1, 2))
  plot(custom_hc, main = "Custom Hierarchical Clustering")
  plot(hclust_res, main = "hclust Function")
} else {
  warning("Not enough significant genes for clustering.")
}



