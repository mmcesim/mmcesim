/**
 * @file channel_graph.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Graph Analysis for Complex Cascaded Channels
 * @version 0.2.2
 * @date 2024-01-14
 *
 * @copyright Copyright (c) 2022-2024 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_CHANNEL_GRAPH_H_
#define _EXPORT_CHANNEL_GRAPH_H_

#include "log_global.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Channel_Graph {
  public:
    /**
     * @brief Add channel from configuration to the channel graph.
     *
     * @param id The channel id.
     * @param from The from node of the channel.
     * @param to The to mode of the channel.
     * @retval true The channel is successfully added.
     * @retval false There channel fails to be added correctly.
     */
    bool addChannel(const std::string& id, const std::string& from, const std::string& to);

    size_t nodeIndex(const std::string& id) const;

    size_t channelIndex(const std::string& id) const;

    /**
     * @brief Get the 'from' node index of channel
     *
     * @param id The channel id
     * @return (size_t) the node index
     */
    size_t chFromIndex(const std::string& id) const;

    /**
     * @brief Get the 'to' node index of channel
     *
     * @param id The channel id
     * @return (size_t) the node index
     */
    size_t chToIndex(const std::string& id) const;

    /**
     * @brief The number of paths in the channel graph.
     *
     * @return (size_t) the number of paths
     */
    size_t pathsNum() const noexcept;

    size_t maxPathLength() const;

    bool arrange();

  private:
    /**
     * @brief Form paths based on the current frontier.
     *
     * @param path The current path.
     */
    void _formPaths(const std::vector<size_t>& path);

    /**
     * @brief Validate paths.
     *
     * @details This removes all paths that does not terminate in the Rx.
     * @deprecated This is not used anymore since everything is handled well in _formPaths now.
     */
    void _validatePaths();

  public:
    static constexpr const size_t MAX_INDEX = -1;

    std::vector<std::string> nodes;
    std::vector<std::string> channels;
    std::vector<size_t> from;
    std::vector<size_t> to;
    std::vector<size_t> Tx;
    std::vector<size_t> Rx;
    std::vector<std::vector<size_t>> paths; /**< each path are stored as channel indices */
    std::vector<size_t> paths_num_acc;
    std::vector<std::pair<size_t, size_t>> paths_indices; /**< map 1D index to 2D used in paths */
};

inline size_t Channel_Graph::chFromIndex(const std::string& id) const { return from[channelIndex(id)]; }

inline size_t Channel_Graph::chToIndex(const std::string& id) const { return to[channelIndex(id)]; }

inline size_t Channel_Graph::pathsNum() const noexcept { return paths.size(); }

inline size_t Channel_Graph::maxPathLength() const {
    size_t max = 0;
    for (const auto& path : paths) max = std::max(max, path.size());
    return max;
}

#endif
