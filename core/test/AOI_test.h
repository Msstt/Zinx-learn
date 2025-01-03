#include "core/AOI.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(AOI, GetSurroundGrids) {
  auto aoi = AOIManager(0, 250, 5, 0, 250, 5);

  auto grid_ids = aoi.GetSurroundGrids(20);
  ASSERT_THAT(grid_ids, ::testing::ElementsAre(15, 16, 20, 21));

  grid_ids = aoi.GetSurroundGrids(9);
  ASSERT_THAT(grid_ids, ::testing::ElementsAre(3, 4, 8, 9, 13, 14));

  grid_ids = aoi.GetSurroundGrids(6);
  ASSERT_THAT(grid_ids, ::testing::ElementsAre(0, 1, 2, 5, 6, 7, 10, 11, 12));
}