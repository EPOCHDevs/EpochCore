//
// Created by adesola on 1/3/25.
//

#pragma once
#include "enum_wrapper.h"


CREATE_ENUM(EpochLabErrorType,
                   CampaignOperationFailed,
                    CampaignInterrupted,
                    CampaignInvalidState,
                    CampaignNotFound,
                    CampaignUnexpectedFailure,
                   YamlParseError,        // Error parsing YAML
                   ValidationFailed,      // YAML validation error
                   DBOperationFailed,     // DynamoDB operation failed
                    DBPutFailed,     // DynamoDB Put failed
                    DBDeleteFailed,     // DynamoDB Delete failed
                    DBGetFailed,     // DynamoDB Get failed
                    DBUpdateFailed,     // DynamoDB Update failed
                    DataNotFound,     // DataNotFound
                    InvalidMetaData,     // InvalidMetaData
                    InternalAssetId,   // Internal Asset Id
                    InternalServerError,   // Unhandled exceptions
                   UnauthorizedAccess,    // Missing or invalid userId
                   BadRequest,             // General client error
                   GlazeSerializationError, // Error Serializing with Glaze
                    GlazeDeSerializationError // Error DeSerializing with Glaze
);
