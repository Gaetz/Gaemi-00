//
// Created by gaetz on 07/01/24.
//

#include "QueryManager.hpp"
#include "World.hpp"

namespace gecs {

    using ComponentArchetypes = unordered_map<ArchetypeId, size_t>;

    vector<IdArchRow> QueryManager::GetEntitiesWithArchsRows() {
        vector<IdArchRow> entitiesWithArchRows;
        for (const auto &entity: World::Instance().GetEntities()) {
            entitiesWithArchRows.push_back(
                    IdArchRow{entity.first, entity.second.archetype->archetypeId, entity.second.row});
        }
        return entitiesWithArchRows;
    }

    /**
     * Check all archetypes by components to obtain component, archetypes and cols
     * @param componentIds Relevant components
     * @param pattern Minimal pattern for archetypes
     * @return Vector of relevant component/archetype/column index data, ordered by component then archetype
     */
    vector<CompArchIdAndCol> QueryManager::GetComponentsWithArchsCols(vector<ComponentId>&& componentIds, std::bitset<32> pattern) {

        vector<CompArchIdAndCol> res;
        World& world = World::Instance();
        for (const auto& c : world.GetComponents()) {
            if (std::find(componentIds.begin(), componentIds.end(), c.first) == componentIds.end()) continue;
            ComponentArchetypes archs = c.second;
            for (const auto archIds : archs) {
                // We check the archetype contain the minimal components
                // We also exclude empty archetypes
                if ((archIds.first & pattern) == pattern && (world.GetArchetypes())[archIds.first].GetRowCount() > 0) {
                    res.push_back(CompArchIdAndCol { c.first, archIds.first, archIds.second });
                }
            }
        }

        std::sort(res.begin(), res.end(), [=](CompArchIdAndCol a, CompArchIdAndCol b) {
            return a.componentId < b.componentId ||
                   (a.componentId == b.componentId && a.archId.to_ulong() < b.archId.to_ulong());
        });

        return res;
    }

    Column QueryManager::GetColumn(ArchetypeId archId, size_t column) {
        return (World::Instance().GetArchetypes())[archId].components[column];
    }

    vector<CompArchIdAndCol> QueryManager::FilterForOneComponent(const vector<CompArchIdAndCol>& filterMaterial, ComponentId componentId) {
        vector<CompArchIdAndCol> compFilter;
        for (const auto& compArchCol: filterMaterial) {
            if (compArchCol.componentId == componentId) {
                compFilter.emplace_back(compArchCol);
            }
        }
        return compFilter;
    }
}