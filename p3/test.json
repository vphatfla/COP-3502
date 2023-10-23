import json
import boto3

def lambda_handler(event, context):
    
    sqs = boto3.client('sqs')
    reko=boto3.client('rekognition')
    
    for record in event['Records']:
        # Extract the message body
        message_body = json.loads(record['body'])
        
        # Your message processing logic goes here
        # For example, you can print the message
        
        for record in message_body['Records']:
            s3Info = record['s3']
            s3BucketName = s3Info['bucket']['name']
            s3ObjectKey = s3Info['object']['key']
            parts = s3ObjectKey.split('/')
            s3FolderName = parts[0]
            s3ObjectName = parts[1]
            if (s3ObjectName == "init.png"):
                print("This is init image!")
            else:
                initFileKey = s3FolderName + '/init'
                resp = reko.compare_faces(
                    SourceImage={
                    'S3Object': {
                        'Bucket': s3BucketName,
                        'Name': initFileKey,
                    }},
                    TargetImage={
                    'S3Object': {
                        'Bucket': s3BucketName,
                        'Name': s3ObjectKey,
                    }}
                )
            
                print(resp)
                print("Bucket name = " + s3BucketName)
                print("Object key = "  + s3ObjectKey)

    return {
        'statusCode': 200,
        'body': json.dumps('Messages processed successfully')
    }

