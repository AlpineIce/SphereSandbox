// Copyright 2017 The Draco Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "draco/compression/decode.h"

#include <cinttypes>
#include <sstream>

#include "draco/compression/encode.h"
#include "draco/core/draco_test_base.h"
#include "draco/core/draco_test_utils.h"
#include "draco/io/file_utils.h"
#include "draco/io/obj_encoder.h"

namespace {

class DecodeTest : public ::testing::Test {
 protected:
  DecodeTest() {}
};

#ifdef DRACO_BACKWARDS_COMPATIBILITY_SUPPORTED
TEST_F(DecodeTest, TestSkipAttributeTransform) {
  const std::string file_name = "test_nm_quant.0.9.0.drc";
  // Tests that decoders can successfully skip attribute transform.
  std::vector<char> data;
  ASSERT_TRUE(
      draco::ReadFileToBuffer(draco::GetTestFileFullPath(file_name), &data));
  ASSERT_FALSE(data.empty());

  // Create a draco decoding buffer. Note that no data is copied in this step.
  draco::DecoderBuffer buffer;
  buffer.Init(data.data(), data.size());

  draco::Decoder decoder;
  // Make sure we skip dequantization for the position attribute.
  decoder.SetSkipAttributeTransform(draco::GeometryAttribute::POSITION);

  // Decode the input data into a geometry.
  std::unique_ptr<draco::PointCloud> pc =
      decoder.DecodePointCloudFromBuffer(&buffer).value();
  ASSERT_NE(pc, nullptr);

  const draco::PointAttribute *const pos_att =
      pc->GetNamedAttribute(draco::GeometryAttribute::POSITION);
  ASSERT_NE(pos_att, nullptr);

  // Ensure the position attribute is of type int32_t and that it has a valid
  // attribute transform.
  ASSERT_EQ(pos_att->data_type(), draco::DT_INT32);
  ASSERT_NE(pos_att->GetAttributeTransformData(), nullptr);

  // Normal attribute should be left transformed.
  const draco::PointAttribute *const norm_att =
      pc->GetNamedAttribute(draco::GeometryAttribute::NORMAL);
  ASSERT_EQ(norm_att->data_type(), draco::DT_FLOAT32);
  ASSERT_EQ(norm_att->GetAttributeTransformData(), nullptr);
}
#endif

void TestSkipAttributeTransformOnPointCloudWithColor(const std::string &file) {
  std::vector<char> data;
  ASSERT_TRUE(draco::ReadFileToBuffer(draco::GetTestFileFullPath(file), &data));
  ASSERT_FALSE(data.empty());

  // Create a draco decoding buffer. Note that no data is copied in this step.
  draco::DecoderBuffer buffer;
  buffer.Init(data.data(), data.size());

  draco::Decoder decoder;
  // Make sure we skip dequantization for the position attribute.
  decoder.SetSkipAttributeTransform(draco::GeometryAttribute::POSITION);

  // Decode the input data into a geometry.
  std::unique_ptr<draco::PointCloud> pc =
      decoder.DecodePointCloudFromBuffer(&buffer).value();
  ASSERT_NE(pc, nullptr);

  const draco::PointAttribute *const pos_att =
      pc->GetNamedAttribute(draco::GeometryAttribute::POSITION);
  ASSERT_NE(pos_att, nullptr);

  // Ensure the position attribute is of type int32_t or uint32_t and that it
  // has a valid attribute transform.
  ASSERT_TRUE(pos_att->data_type() == draco::DT_INT32 ||
              pos_att->data_type() == draco::DT_UINT32);
  ASSERT_NE(pos_att->GetAttributeTransformData(), nullptr);

  const draco::PointAttribute *const clr_att =
      pc->GetNamedAttribute(draco::GeometryAttribute::COLOR);
  ASSERT_EQ(clr_att->data_type(), draco::DT_UINT8);

  // Ensure the color attribute was decoded correctly. Perform the decoding
  // again without skipping the position dequantization and compare the
  // attribute values.

  draco::DecoderBuffer buffer_2;
  buffer_2.Init(data.data(), data.size());

  draco::Decoder decoder_2;

  // Decode the input data into a geometry.
  std::unique_ptr<draco::PointCloud> pc_2 =
      decoder_2.DecodePointCloudFromBuffer(&buffer_2).value();
  ASSERT_NE(pc_2, nullptr);

  const draco::PointAttribute *const clr_att_2 =
      pc_2->GetNamedAttribute(draco::GeometryAttribute::COLOR);
  ASSERT_NE(clr_att_2, nullptr);
  for (draco::PointIndex pi(0); pi < pc_2->num_points(); ++pi) {
    // Colors should be exactly the same for both cases.
    ASSERT_EQ(std::memcmp(clr_att->GetAddress(clr_att->mapped_index(pi)),
                          clr_att_2->GetAddress(clr_att_2->mapped_index(pi)),
                          clr_att->byte_stride()),
              0);
  }
}

TEST_F(DecodeTest, TestSkipAttributeTransformOnPointCloud) {
  // Tests that decoders can successfully skip attribute transform on a point
  // cloud with multiple attributes encoded with one attributes encoder.
  TestSkipAttributeTransformOnPointCloudWithColor("pc_color.drc");
  TestSkipAttributeTransformOnPointCloudWithColor("pc_kd_color.drc");
}

TEST_F(DecodeTest, TestSkipAttributeTransformWithNoQuantization) {
  // Tests that decoders can successfully skip attribute transform even though
  // the input model was not quantized (it has no attribute transform).
  const std::string file_name = "point_cloud_no_qp.drc";
  std::vector<char> data;
  ASSERT_TRUE(
      draco::ReadFileToBuffer(draco::GetTestFileFullPath(file_name), &data));
  ASSERT_FALSE(data.empty());

  // Create a draco decoding buffer. Note that no data is copied in this step.
  draco::DecoderBuffer buffer;
  buffer.Init(data.data(), data.size());

  draco::Decoder decoder;
  // Make sure we skip dequantization for the position attribute.
  decoder.SetSkipAttributeTransform(draco::GeometryAttribute::POSITION);

  // Decode the input data into a geometry.
  std::unique_ptr<draco::PointCloud> pc =
      decoder.DecodePointCloudFromBuffer(&buffer).value();
  ASSERT_NE(pc, nullptr);

  const draco::PointAttribute *const pos_att =
      pc->GetNamedAttribute(draco::GeometryAttribute::POSITION);
  ASSERT_NE(pos_att, nullptr);

  // Ensure the position attribute is of type float32 since the attribute was
  // not quantized.
  ASSERT_EQ(pos_att->data_type(), draco::DT_FLOAT32);

  // Make sure there is no attribute transform available for the attribute.
  ASSERT_EQ(pos_att->GetAttributeTransformData(), nullptr);
}

TEST_F(DecodeTest, TestSkipAttributeTransformUniqueId) {
  // Tests that decoders preserve unique id of attributes even when their
  // attribute transforms are skipped.
  const std::string file_name = "cube_att.obj";
  auto src_mesh = draco::ReadMeshFromTestFile(file_name);
  ASSERT_NE(src_mesh, nullptr);

  constexpr int kPosUniqueId = 7;
  constexpr int kNormUniqueId = 42;
  // Set unique ids for some of the attributes.
  src_mesh
      ->attribute(
          src_mesh->GetNamedAttributeId(draco::GeometryAttribute::POSITION))
      ->set_unique_id(kPosUniqueId);
  src_mesh
      ->attribute(
          src_mesh->GetNamedAttributeId(draco::GeometryAttribute::NORMAL))
      ->set_unique_id(kNormUniqueId);

  draco::EncoderBuffer encoder_buffer;
  draco::Encoder encoder;
  encoder.SetAttributeQuantization(draco::GeometryAttribute::POSITION, 10);
  encoder.SetAttributeQuantization(draco::GeometryAttribute::NORMAL, 11);
  encoder.EncodeMeshToBuffer(*src_mesh, &encoder_buffer);

  // Create a draco decoding buffer.
  draco::DecoderBuffer buffer;
  buffer.Init(encoder_buffer.data(), encoder_buffer.size());

  // First we decode the mesh without skipping the attribute transforms.
  draco::Decoder decoder_no_skip;
  std::unique_ptr<draco::Mesh> mesh_no_skip =
      decoder_no_skip.DecodeMeshFromBuffer(&buffer).value();
  ASSERT_NE(mesh_no_skip, nullptr);

  // Now we decode it again while skipping some attributes.
  draco::Decoder decoder_skip;
  // Make sure we skip dequantization for the position and normal attribute.
  decoder_skip.SetSkipAttributeTransform(draco::GeometryAttribute::POSITION);
  decoder_skip.SetSkipAttributeTransform(draco::GeometryAttribute::NORMAL);

  // Decode the input data into a geometry.
  buffer.Init(encoder_buffer.data(), encoder_buffer.size());
  std::unique_ptr<draco::Mesh> mesh_skip =
      decoder_skip.DecodeMeshFromBuffer(&buffer).value();
  ASSERT_NE(mesh_skip, nullptr);

  // Compare the unique ids.
  const draco::PointAttribute *const pos_att_no_skip =
      mesh_no_skip->GetNamedAttribute(draco::GeometryAttribute::POSITION);
  ASSERT_NE(pos_att_no_skip, nullptr);
  ASSERT_EQ(pos_att_no_skip->data_type(), draco::DataType::DT_FLOAT32);

  const draco::PointAttribute *const pos_att_skip =
      mesh_skip->GetNamedAttribute(draco::GeometryAttribute::POSITION);
  ASSERT_NE(pos_att_skip, nullptr);
  ASSERT_EQ(pos_att_skip->data_type(), draco::DataType::DT_INT32);

  const draco::PointAttribute *const norm_att_no_skip =
      mesh_no_skip->GetNamedAttribute(draco::GeometryAttribute::NORMAL);
  ASSERT_NE(norm_att_no_skip, nullptr);
  ASSERT_EQ(norm_att_no_skip->data_type(), draco::DataType::DT_FLOAT32);

  const draco::PointAttribute *const norm_att_skip =
      mesh_skip->GetNamedAttribute(draco::GeometryAttribute::NORMAL);
  ASSERT_NE(norm_att_skip, nullptr);
  ASSERT_EQ(norm_att_skip->data_type(), draco::DataType::DT_INT32);

  ASSERT_EQ(pos_att_skip->unique_id(), pos_att_no_skip->unique_id());
  ASSERT_EQ(norm_att_skip->unique_id(), norm_att_no_skip->unique_id());
  std::cout << pos_att_skip->unique_id() << " " << norm_att_skip->unique_id()
            << std::endl;
}

}  // namespace
