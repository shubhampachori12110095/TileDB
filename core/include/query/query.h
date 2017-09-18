/**
 * @file   query.h
 *
 * @section LICENSE
 *
 * The MIT License
 *
 * @copyright Copyright (c) 2017 TileDB, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * This file defines class Query.
 */

#ifndef TILEDB_QUERY_H
#define TILEDB_QUERY_H

#include "array_read_state.h"
#include "array_sorted_read_state.h"
#include "array_sorted_write_state.h"
#include "fragment.h"
#include "query_status.h"
#include "query_type.h"
#include "status.h"
#include "storage_manager.h"

#include <vector>

namespace tiledb {

class ArrayReadState;
class ArraySortedReadState;
class ArraySortedWriteState;
class Fragment;
class StorageManager;

/** Processes a (read/write) query. */
class Query {
 public:
  /* ********************************* */
  /*     CONSTRUCTORS & DESTRUCTORS    */
  /* ********************************* */

  /** Constructor. */
  Query();

  /** Destructor. */
  ~Query();

  /* ********************************* */
  /*                 API               */
  /* ********************************* */

  /** Adds the coordinates attribute if it does not exist. */
  void add_coords();

  /** Returns the array schema.*/
  const ArraySchema* array_schema() const;

  /** Processes asynchronously the query. */
  Status async_process();

  /** Returns the list of ids of attributes involved in the query. */
  const std::vector<unsigned int>& attribute_ids() const;

  /** Finalizes and deletes the created fragments. */
  Status clear_fragments();

  /**
   * Retrieves the index of the coordinates buffer in the specified query
   * buffers.
   *
   * @param coords_buffer_i The index of the coordinates buffer to be retrieved.
   * @return Status
   */
  Status coords_buffer_i(int* coords_buffer_i) const;

  /**
   * Finalizes the query, properly finalizing and deleting the involved
   * fragments.
   */
  Status finalize();

  /** Returns the fragments involved in the query. */
  const std::vector<Fragment*>& fragments() const;

  /** Returns the metadata of the fragments involved in the query. */
  const std::vector<FragmentMetadata*>& fragment_metadata() const;

  /** Returns the number of fragments involved in the query. */
  unsigned int fragment_num() const;

  /**
   * Initializes the query.
   *
   * @param storage_manager The storage manager.
   * @param array_schema The array schema.
   * @param fragment_metadata The metadata of the involved fragments.
   * @param type The query type.
   * @param subarray The subarray the query is constrained on. A nuullptr
   *     indicates the full domain.
   * @param attributes The names of the attributes involved in the query.
   * @param attribute_num The number of attributes.
   * @param buffers The query buffers with a one-to-one correspondences with
   *     the specified attributes. In a read query, the buffers will be
   *     populated with the query results. In a write query, the buffer
   *     contents will be appropriately written in a new fragment.
   * @param buffer_sizes The corresponding buffer sizes.
   * @return Status
   */
  Status init(
      StorageManager* storage_manager,
      const ArraySchema* array_schema,
      const std::vector<FragmentMetadata*>& fragment_metadata,
      QueryType type,
      const void* subarray,
      const char** attributes,
      unsigned int attribute_num,
      void** buffers,
      uint64_t* buffer_sizes);

  /**
   * Initializes the query.
   *
   * @param storage_manager The storage manager.
   * @param array_schema The array schema.
   * @param fragment_metadata The metadata of the involved fragments.
   * @param type The query type.
   * @param subarray The subarray the query is constrained on. A nuullptr
   *     indicates the full domain.
   * @param attributes_ids The ids of the attributes involved in the query.
   * @param buffers The query buffers with a one-to-one correspondences with
   *     the specified attributes. In a read query, the buffers will be
   *     populated with the query results. In a write query, the buffer
   *     contents will be appropriately written in a new fragment.
   * @param buffer_sizes The corresponding buffer sizes.
   * @param add_coords If *true*, the coordinates attribute will be added
   *     to the provided *attribute_ids*. This is important for internal async
   *     read queries on sparse arrays, where the user had not specified
   *     the retrieval of the coordinates.
   * @return Status
   */
  Status init(
      StorageManager* storage_manager,
      const ArraySchema* array_schema,
      const std::vector<FragmentMetadata*>& fragment_metadata,
      QueryType type,
      const void* subarray,
      const std::vector<unsigned int>& attribute_ids,
      void** buffers,
      uint64_t* buffer_sizes,
      bool add_coords = false);

  /**
   * Returns true if the query cannot write to some buffer due to
   * an overflow.
   */
  bool overflow() const;

  /**
   * Checks if a particular query buffer (corresponding to some attribute)
   * led to an overflow based on an attribute id.
   */
  bool overflow(int attribute_id) const;

  /**
   * Checks if a particular query buffer (corresponding to some attribute)
   * led to an overflow based on an attribute name.
   *
   * @param attribute_name The attribute whose overflow to retrieve.
   * @param overflow The overflow status to be retieved.
   * @return Status (error is attribute is not involved in the query).
   */
  Status overflow(const char* attribute_name, unsigned int* overflow) const;

  /** Executes a read query. */
  Status read();

  /**
   * Executes a read query, but the query retrieves cells in the global
   * cell order, and also the results are written in the input buffers,
   * not the internal buffers.
   */
  Status read(void** buffers, uint64_t* buffer_sizes);

  /** Sets the query buffers. */
  void set_buffers(void** buffers, uint64_t* buffer_sizes);

  /**
   * Sets the callback function and its data input that will be called
   * upon the completion of an asynchronous query.
   */
  void set_callback(void* (*callback)(void*), void* callback_data);

  /** Sets the query status. */
  void set_status(QueryStatus status);

  /** Returns the query status. */
  QueryStatus status() const;

  /** Returns the storage manager. */
  StorageManager* storage_manager() const;

  /** Returns the subarray in which the query is constrained. */
  const void* subarray() const;

  /** Returns the query type. */
  QueryType type() const;

  /** Executes a write query. */
  Status write();

  /**
   * Executes a write query, but the query writes the cells in the global
   * cell order, and also the cells are read from the input buffers,
   * not the internal buffers.
   */
  Status write(void** buffers, uint64_t* buffer_sizes);

 private:
  /* ********************************* */
  /*         PRIVATE ATTRIBUTES        */
  /* ********************************* */

  /** The array schema. */
  const ArraySchema* array_schema_;

  /**
   * The array read state. Handles reads in the presence of multiple
   * fragments. It returns results ordered in the global cell order.
   */
  ArrayReadState* array_read_state_;

  /**
   * The array sorted read state. It handles read queries that must
   * return the results ordered in a particular layout other than
   * the global cell order.
   */
  ArraySortedReadState* array_sorted_read_state_;

  /**
   * The araay sorted write state. It handles write queries that
   * must write cells provided in a layout that is different
   * than the global cell order.
   */
  ArraySortedWriteState* array_sorted_write_state_;

  /** The ids of the attributes involved in the query. */
  std::vector<unsigned int> attribute_ids_;

  /**
   * The query buffers (one per involved attribute, two per variable-sized
   * attribute.
   */
  void** buffers_;

  /** The corresponding buffer sizes. */
  uint64_t* buffer_sizes_;

  /** A function that will be called upon the completion of an async query. */
  void* (*callback_)(void*);

  /** The data input to the callback function. */
  void* callback_data_;

  /** The query status. */
  QueryStatus status_;

  /** The fragments involved in the query. */
  std::vector<Fragment*> fragments_;

  /** The metadata of the fragments involved in the query. */
  std::vector<FragmentMetadata*> fragment_metadata_;

  /** The storage manager. */
  StorageManager* storage_manager_;

  /**
   * The subarray the query is constrained on. A nullptr implies the
   * entire domain.
   */
  void* subarray_;

  /** The query type. */
  QueryType type_;

  /* ********************************* */
  /*           PRIVATE METHODS         */
  /* ********************************* */

  /** Initializes the fragments (for a read query). */
  Status init_fragments(
      const std::vector<FragmentMetadata*>& fragment_metadata);

  /** Initializes the query states. */
  Status init_states();

  /** Creates a new fragment (for a write query). */
  Status new_fragment();

  /**
   * Returns a new fragment name, which is in the form: <br>
   * .__thread-id_timestamp. For instance,
   *  __6426153_1458759561320
   *
   * Note that this is a temporary name, initiated by a new write process.
   * After the new fragmemt is finalized, the array will change its name
   * by removing the leading '.' character.
   *
   * @return A new special fragment name on success, or "" (empty string) on
   *     error.
   */
  std::string new_fragment_name() const;

  /**
   * Opens the existing fragments.
   *
   * @param metadata The metadata of the array fragments.
   * @return Status
   */
  Status open_fragments(const std::vector<FragmentMetadata*>& metadata);

  /** Sets the query attributes. */
  Status set_attributes(const char** attributes, unsigned int attribute_num);

  /** Sets the query subarray. */
  Status set_subarray(const void* subarray);
};

}  // namespace tiledb

#endif  // TILEDB_QUERY_H
