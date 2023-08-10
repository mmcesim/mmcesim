/**
 * @file channel_graph.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Graph Analysis for Complex Cascaded Channels
 * @version 0.2.2
 * @date 2023-03-18
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_CHANNEL_GRAPH_H_
#define _EXPORT_CHANNEL_GRAPH_H_

#include "log_global.h"
#include "utils.h"
#include <iostream>
#include <string>
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

    unsigned nodeIndex(const std::string& id) const;

    unsigned channelIndex(const std::string& id) const;

    /**
     * @brief Get the 'from' node index of channel
     *
     * @param id The channel id
     * @return (unsigned) the node index
     */
    unsigned chFromIndex(const std::string& id) const;

    /**
     * @brief Get the 'to' node index of channel
     *
     * @param id The channel id
     * @return (unsigned) the node index
     */
    unsigned chToIndex(const std::string& id) const;

    unsigned pathsNum() const noexcept;

    bool arrange();

  private:
    /**
     * @brief Form paths based on the current frontier.
     *
     * @param path The current paths.
     */
    void _formPaths(const std::vector<unsigned>& path);

    /**
     * @brief Validate paths.
     *
     * @details This removes all paths that does not terminate in the Rx.
     * @deprecated This is not used anymore since everything is handled well in _formPaths now.
     */
    void _validatePaths();

  public:
    static constexpr const unsigned MAX_INDEX = -1;

    std::vector<std::string> nodes;
    std::vector<std::string> channels;
    std::vector<unsigned> from;
    std::vector<unsigned> to;
    std::vector<unsigned> Tx;
    std::vector<unsigned> Rx;
    std::vector<std::vector<unsigned>> paths;
};

inline unsigned Channel_Graph::chFromIndex(const std::string& id) const { return from[channelIndex(id)]; }

inline unsigned Channel_Graph::chToIndex(const std::string& id) const { return to[channelIndex(id)]; }

inline unsigned Channel_Graph::pathsNum() const noexcept { return paths.size(); }

#endif
