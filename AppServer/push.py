import logging

from apiclient.errors import HttpError


def WatchChange(service, channel_id, channel_type, channel_address,
                channel_token=None, channel_params=None):
    """Watch for all changes to a user's Drive.

    Args:
      service: Drive API service instance.
      channel_id: Unique string that identifies this channel.
      channel_type: Type of delivery mechanism used for this channel.
      channel_address: Address where notifications are delivered.
      channel_token: An arbitrary string delivered to the target address with
                     each notification delivered over this channel. Optional.
      channel_address: Address where notifications are delivered. Optional.

    Returns:
      The created channel if successful
    Raises:
      apiclient.errors.HttpError: if http request to create channel fails.
    """
    body = {
        'id': channel_id,
        'type': channel_type,
        'address': channel_address
    }
    if channel_token:
        body['token'] = channel_token
    if channel_params:
        body['params'] = channel_params
    return service.changes().watch(body=body).execute()


def RetrieveChange(service, change_id):
    """
    Print a single Change resource information.

    Args:
      service: Drive API service instance.
      change_id: ID of the Change resource to retrieve.
    """
    result = {}
    try:
        change = service.changes().get(changeId=change_id).execute()
        result['modificationDate'] = change['modificationDate']
        if change.get('file'):
            file = change['file']
            result['title'] = file['title']
            result['URL'] = file['alternateLink']
            if file.get('lastModifyingUser'):
                user = file['lastModifyingUser']
                result['user_name'] = user['displayName']
                if user.get('picture') and user['picture'].get('url'):
                    result['picture'] = user['picture']['url']
    except HttpError, error:
        logging.error(error)
    return result


def StopChannel(service, channel_id, resource_id):
    """Stop watching to a specific channel.

    Args:
      service: Drive API service instance.
      channel_id: ID of the channel to stop.
      resource_id: Resource ID of the channel to stop.
    Raises:
      apiclient.errors.HttpError: if http request to create channel fails.
    """
    body = {
        'id': channel_id,
        'resourceId': resource_id
    }
    service.channels().stop(body=body).execute()
