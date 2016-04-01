/*
 * File: tiledb_array_update_dense_2.cc
 * 
 * It shows how to update a dense array, writing random sparse updates.
 */

#include "c_api.h"

int main() {
  // Intialize context with the default configuration parameters
  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Initialize array
  TileDB_Array* tiledb_array;
  tiledb_array_init(
      tiledb_ctx,                                // Context 
      &tiledb_array,                             // Array object
      "my_workspace/dense_arrays/my_array_A",    // Array name
      TILEDB_ARRAY_WRITE_UNSORTED,               // Mode
      NULL,                                      // Whole domain (ignored)
      NULL,                                      // All attributes
      0);                                        // Number of attributes

  // Prepare cell buffers
  int buffer_a1[] = { 211,  213,  212,  208 };
  size_t buffer_a2[] = { 0,  4,  6,  7 };
  const char buffer_var_a2[] = "wwwwyyxu";
  float buffer_a3[] = 
      { 211.1, 211.2, 213.1, 213.2, 212.1, 212.2, 208.1, 208.2 };
  int64_t buffer_coords[] = { 4, 2, 3, 4, 3, 3, 3, 1 };
  const void* buffers[] = 
      { buffer_a1, buffer_a2, buffer_var_a2, buffer_a3, buffer_coords };
  size_t buffer_sizes[] = 
  { 
      sizeof(buffer_a1),  
      sizeof(buffer_a2),
      sizeof(buffer_var_a2)-1,  // No need to store the last '\0' character
      sizeof(buffer_a3),
      sizeof(buffer_coords)
  };

  // Write to array
  tiledb_array_write(tiledb_array, buffers, buffer_sizes); 

  // Finalize array
  tiledb_array_finalize(tiledb_array);

  // Finalize context
  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}