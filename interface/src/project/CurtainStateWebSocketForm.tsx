import { FC } from 'react';

import { Switch } from '@mui/material';

import { WEB_SOCKET_ROOT } from '../api/endpoints';
import { BlockFormControlLabel, FormLoader, MessageBox, SectionContent } from '../components';
import { updateValue, useWs } from '../utils';

import { CurtainState } from './types';

export const CURTAIN_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "curtainState";

const CurtainStateWebSocketForm: FC = () => {
  const { connected, updateData, data } = useWs<CurtainState>(CURTAIN_SETTINGS_WEBSOCKET_URL);

  const updateFormValue = updateValue(updateData);

  const content = () => {
    if (!connected || !data) {
      return (<FormLoader message="Connecting to WebSocket…" />);
    }
    return (
      <>
        <MessageBox
          level="info"
          message="Öffnen Schließen via WebSockets. Wird direkt übertragen"
          my={2}
        />
        <BlockFormControlLabel
          control={
            <Switch
              name="closed"
              checked={data.closed}
              onChange={updateFormValue}
              color="primary"
            />
          }
          label="Vorgang geschlossen?"
        />
      </>
    );
  };

  return (
    <SectionContent title='WebSocket Example' titleGutter>
      {content()}
    </SectionContent>
  );
};

export default CurtainStateWebSocketForm;
