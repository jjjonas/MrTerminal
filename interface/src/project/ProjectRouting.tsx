import { FC } from 'react';
import { Navigate, Routes, Route } from 'react-router-dom';

import DemoProject from './DemoProject';
import CurtainProject from './CurtainProject';

const ProjectRouting: FC = () => {
  return (
    <Routes>
      {
        // Add the default route for your project below
      }
      <Route path="/*" element={<Navigate to="curtain/information" />} />
      {
        // Add your project page routes below.
      }
      {/* <Route path="demo/*" element={<DemoProject />} /> */}
      <Route path="curtain/*" element={<CurtainProject />} />
    </Routes>
  );
};

export default ProjectRouting;
