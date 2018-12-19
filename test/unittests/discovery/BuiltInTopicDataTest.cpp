/*
 *
 * Author: Andreas Wüstenberg (andreas.wuestenberg@rwth-aachen.de)
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ucdr/microcdr.h>
#include "rtps/common/Locator.h"

#include "rtps/common/types.h"
#include "rtps/discovery/BuiltInTopicData.h"

using rtps::BuiltInTopicData;

class TopicData : public ::testing::Test{
protected:
    // Extracted data from wireshark
    uint8_t parameterListFromEprosima[260] = {
                                                           0x2f, 0x00, 0x18, 0x00, 0x01, 0x00, 0x00, 0x00, 0xf3, 0x1c,
            /*0080*/   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8,
            /*0090*/   0x00, 0x2d, 0x50, 0x00, 0x10, 0x00, 0x01, 0x0f, 0x00, 0x2d, 0x08, 0x58, 0x00, 0x00, 0x00, 0x00,
            /*00a0*/   0x00, 0x00, 0x00, 0x00, 0x01, 0xc1, 0x05, 0x00, 0x14, 0x00, 0x10, 0x00, 0x00, 0x00, 0x48, 0x65,
            /*00b0*/   0x6c, 0x6c, 0x6f, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x54, 0x6f, 0x70, 0x69, 0x63, 0x00, 0x07, 0x00,
            /*00c0*/   0x10, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x57, 0x6f, 0x72, 0x6c, 0x64,
            /*00d0*/   0x00, 0x00, 0x70, 0x00, 0x10, 0x00, 0x01, 0x0f, 0x00, 0x2d, 0x08, 0x58, 0x00, 0x00, 0x00, 0x00,
            /*00e0*/   0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x5a, 0x00, 0x10, 0x00, 0x01, 0x0f, 0x00, 0x2d, 0x08, 0x58,
            /*00f0*/   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x60, 0x00, 0x04, 0x00, 0x0c, 0x01,
            /*0100*/   0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x02, 0x01, 0x00, 0x00, 0x16, 0x00, 0x04, 0x00, 0x01, 0x0f,
            /*0110*/   0x00, 0x00, 0x1d, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x23, 0x00, 0x08, 0x00, 0xff, 0xff,
            /*0120*/   0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x27, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            /*0130*/   0x00, 0x00, 0x1b, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff,
            /*0140*/   0xff, 0xff, 0x1a, 0x00, 0x0c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0xff,
            /*0150*/   0xff, 0xff, 0x2b, 0x00, 0x08, 0x00, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x00,
            /*0160*/   0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x00,
            /*0170*/   0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00

    };

    ucdrBuffer buffer;

    rtps::Locator expectedUnicastLocator = rtps::Locator::createUDPv4Locator(192,168,0,45,7411);
    rtps::Guid expectedGuid = {{0x01,0x0f,0x00,0x2d, 0x08, 0x58,0x00,0x00,0x00,0x00,0x00,0x00},rtps::ENTITYID_BUILD_IN_PARTICIPANT};
    char expectedTopicName[16] = "HelloWorldTopic";
    char expectedTypeName[11] = "HelloWorld";
    rtps::Guid expectedKey = expectedGuid;
    rtps::Guid expectedEndpointGuid = {{0x01, 0x0f, 0x00, 0x2d, 0x08, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                       {{0x00, 0x00, 0x01}, rtps::EntityKind_t::USER_DEFINED_WRITER_WITHOUT_KEY}};
    rtps::ReliabilityKind_t expectedReliability = rtps::ReliabilityKind_t::RELIABLE;
    uint16_t expectedTypeSizdMax = 268;
    rtps::ProtocolVersion_t expectedProtocolVersion = rtps::PROTOCOLVERSION_2_1;
    rtps::VendorId_t expectedVendorId = {1,15};



    void SetUp() override{
        buffer.endianness = UCDR_LITTLE_ENDIANNESS;
        ucdr_init_buffer(&buffer, parameterListFromEprosima, sizeof(parameterListFromEprosima)/sizeof(parameterListFromEprosima[0]));
    }
};

TEST_F(TopicData, returnTrueOnValidPacket){
    BuiltInTopicData topicData;
    bool success = topicData.readFromUcdrBuffer(buffer);
    EXPECT_TRUE(success);
}

TEST_F(TopicData, readFromUcdrBuffer_setsCorrectValues){
    BuiltInTopicData topicData;
    topicData.readFromUcdrBuffer(buffer);

    constexpr auto sizeTypeName = sizeof(expectedTypeName)/ sizeof(expectedTypeName[0]);
    // TODO Should be switched but couldn't figure out how to match until certain size
    EXPECT_THAT(expectedTypeName, ::testing::ElementsAreArray(topicData.typeName, sizeTypeName));
    constexpr auto sizeTopicName = sizeof(expectedTopicName)/ sizeof(expectedTopicName[0]);
    EXPECT_THAT(expectedTopicName, ::testing::ElementsAreArray(topicData.topicName, sizeTopicName));

    EXPECT_EQ(topicData.reliabilityKind, expectedReliability);
    EXPECT_EQ(topicData.unicastLocator.getIp4Address().addr, expectedUnicastLocator.getIp4Address().addr);
    EXPECT_EQ(topicData.unicastLocator.port, expectedUnicastLocator.port);
    EXPECT_EQ(topicData.unicastLocator.kind, expectedUnicastLocator.kind);
    EXPECT_EQ(topicData.endpointGuid, expectedEndpointGuid);
}
